var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

const ChatRoom = require('./chatroom');

var chatrooms = {
  general: new ChatRoom(io, "general"),
  room01: new ChatRoom(io, "room01"),
  room02: new ChatRoom(io, "room02"),
  room03: new ChatRoom(io, "room03")
}

io.on('connection', function(socket){

  socket.on('disconnect', function(){
    for (var r in socket.rooms) {
      chatrooms[r].disconect(socket);
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

    for (let r in chatrooms) {
      chatrooms[r].updateUserList(socket);
    }

    socket.leaveAll();
    chatrooms["general"].join(socket);
  });

  socket.on('join_room', function(msg){
    for (let r in socket.rooms) {
      chatrooms[r].leave(socket);
    }  

    chatrooms[msg].join(socket);
  });

  socket.on('chat_message', function(msg){
    for (let r in socket.rooms) {
      chatrooms[r].emit('chat_message', msg);
    }
  });
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});