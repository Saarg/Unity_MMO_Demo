// Based on https://github.com/elad/node-cluster-socket.io
const express = require('express');
const cluster = require('cluster');
const net = require('net');
const sio = require('socket.io');
const sio_redis = require('socket.io-redis');
const farmhash = require('farmhash');

const port = 3000;
const num_processes = require('os').cpus().length;

if (cluster.isMaster) {
    console.log("Server started.")

    // This stores our workers. We need to keep them to be able to reference
    // them based on source IP address. It's also useful for auto-restart,
    // for example.
    var workers = [];

    // Helper function for spawning worker at index 'i'.
    var spawn = function (i) {
        workers[i] = cluster.fork();

        // Optional: Restart worker on exit
        workers[i].on('exit', function (code, signal) {
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
    var worker_index = function (ip, len) {
        return farmhash.fingerprint32(ip) % len; // Farmhash is the fastest and works with IPv6, too
    };

    // Create the outside facing server listening on our port.
    var server = net.createServer({ pauseOnConnect: true }, function (connection) {
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
    var server = app.listen(0, 'localhost');
    var io = sio(server);

    // Tell Socket.IO to use the redis adapter. By default, the redis
    // server is assumed to be on localhost:6379. You don't have to
    // specify them explicitly unless you want to change them.
    io.adapter(sio_redis({ host: '51.15.247.163', port: 6379 }));

    const msg_handler = require('./message_handler');
    msg_handler(io);

    // Listen to messages sent from the master. Ignore everything else.
    process.on('message', function (message, connection) {
        if (message == 'sticky-session:connection') {
            // Emulate a connection event on the server by emitting the
            // event with the connection the master sent us.
            server.emit('connection', connection);

            connection.resume();
        }
    });
}