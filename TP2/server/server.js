// Based on https://github.com/elad/node-cluster-socket.io
var express = require('express'),
    cluster = require('cluster'),
    net = require('net'),
    sio = require('socket.io'),
    sio_redis = require('socket.io-redis'),
    farmhash = require('farmhash');

var port = 3000,
    num_processes = 2;//require('os').cpus().length;

if (cluster.isMaster) {
	// This stores our workers. We need to keep them to be able to reference
	// them based on source IP address. It's also useful for auto-restart,
	// for example.
	var workers = [];

	// Helper function for spawning worker at index 'i'.
	var spawn = function(i) {
		workers[i] = cluster.fork();

		// Optional: Restart worker on exit
		workers[i].on('exit', function(code, signal) {
			console.log('respawning worker', i);
			spawn(i);
		});
    };

    // Spawn workers.
	for (var i = 0; i < num_processes; i++) {
		spawn(i);
	}

	// Helper function for getting a worker index based on IP address.
	// This is a hot path so it should be really fast. The way it works
	// is by converting the IP address to a number by removing non numeric
  // characters, then compressing it to the number of slots we have.
	//
	// Compared against "real" hashing (from the sticky-session code) and
	// "real" IP number conversion, this function is on par in terms of
	// worker index distribution only much faster.
	var worker_index = function(ip, len) {
		return farmhash.fingerprint32(ip) % len; // Farmhash is the fastest and works with IPv6, too
	};

	// Create the outside facing server listening on our port.
	var server = net.createServer({ pauseOnConnect: true }, function(connection) {
		// We received a connection and need to pass it to the appropriate
		// worker. Get the worker for this connection's source IP and pass
		// it the connection.
		var worker = workers[worker_index(connection.remoteAddress, num_processes)];
    worker.send('sticky-session:connection', connection);
  }).listen(port);
} else {
    // Note we don't use a port here because the master listens on it for us.
	var app = new express();

	// Here you might use middleware, attach routes, etc.

	// Don't expose our internal server to the outside.
	var server = app.listen(0, 'localhost'),
	    io = sio(server);

	// Tell Socket.IO to use the redis adapter. By default, the redis
	// server is assumed to be on localhost:6379. You don't have to
	// specify them explicitly unless you want to change them.
	io.adapter(sio_redis({ host: 'localhost', port: 6379 }));

  // Here you might use Socket.IO middleware for authorization etc.
  const ChatRoom = require('./chatroom');

  var chatrooms = {
    general: new ChatRoom(io,"general"),
    room01: new ChatRoom(io,"room01"),
    room02: new ChatRoom(io,"room02"),
    room03: new ChatRoom(io,"room03")
  }

  io.on('connection', function(socket){
    socket.on('disconnect', function(){
      for (let room in chatrooms) {
          chatrooms[room].disconect(socket);
      }
    });
  
    socket.on('user_login', function(username){
      for (let room in chatrooms) {
        if (chatrooms[room].findUser(username)) {
          socket.emit("login_refused");
          return;
        }
      }  
  
      socket.username = username;
      console.log(username + ' connected');
  
      socket.leaveAll();
      chatrooms["general"].join(socket);
    });
  
    socket.on('join_room', function(msg){
      io.of('/').adapter.clientRooms(socket.id, (err, rooms) => {
        rooms.forEach(r => {
          chatrooms[r].leave(socket);
        });
      });
                 
      chatrooms[msg].join(socket);
    });
  
    socket.on('chat_message', function(msg){
      for (let r in socket.rooms) {
        chatrooms[socket.rooms[r]].emit('chat_message', msg);
      }
    });
  });

	// Listen to messages sent from the master. Ignore everything else.
	process.on('message', function(message, connection) {
		if (message == 'sticky-session:connection') {
			// Emulate a connection event on the server by emitting the
      // event with the connection the master sent us.
      server.emit('connection', connection);

      connection.resume();
    }
	});
}