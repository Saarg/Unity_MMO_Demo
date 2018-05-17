var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

var chatrooms = {
  general: {
    users: {}
  },
  room01: {
    users: {}
  },
  room02: {
    users: {}
  },
  room03: {
    users: {}
  }
}

io.on('connection', function(socket){


  socket.on('disconnect', function(){
    for (let r in chatrooms) {
      if (!chatrooms[r].users[socket.username])
        continue;

      delete chatrooms[r].users[socket.username];
      console.log(socket.username + ' disconnected from ' + r);
      io.emit("update_userlist", { name: r, users: Object.keys(chatrooms[r].users) });
    }
    
    for (var r in socket.rooms)
      delete chatrooms[r].users[socket.username];
  });

  socket.on('user_login', function(username){
    for (let room in chatrooms) {
      if (chatrooms[room].users[username]) {
        socket.emit("login_refused");
        return;
      }
    }  

    console.log(username + ' connected');

    socket.leaveAll();
    socket.join("general");
    socket.username = username
    chatrooms["general"].users[username] = socket;
    io.emit("update_userlist", { name: "general", users: Object.keys(chatrooms["general"].users) });

    for (let r in chatrooms) {
      socket.emit("update_userlist", { name: r, users: Object.keys(chatrooms[r].users) }); 
    }
  });

  socket.on('join_room', function(msg){
    for (let r in socket.rooms) {
      if (chatrooms[r].users[socket.username]) {
        delete chatrooms[r].users[socket.username];

        io.emit("update_userlist", { name: r, users: Object.keys(chatrooms[r].users) }); 
        console.log(socket.username + " left " + r);        
        socket.leave(r);
      }
    }  

    console.log(socket.username + " joined " + msg);
    chatrooms[msg].users[socket.username] = socket;
    socket.join(msg);
    io.emit("update_userlist", { name: msg, users: Object.keys(chatrooms[msg].users) }); 
    socket.emit("clear_chat");
  });

  socket.on('chat_message', function(msg){
    for (let r in socket.rooms) {
      io.in(r).emit('chat_message', msg);
    }
  });
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});