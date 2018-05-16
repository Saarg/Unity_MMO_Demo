var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

var users = {};

io.on('connection', function(socket){
  socket.on('disconnect', function(){

    for (var name in users){
      if (users.hasOwnProperty(name) && users[name] == socket) {
        io.emit('user_logout', name);
        console.log(name + ' disconnected');

        delete users[name];
        break;
      }
    }
  });

  socket.on('user_login', function(msg){
    if (users[msg]) {
      socket.emit("login_refused");
    } else {
      users[msg] = socket;
      io.emit('user_login', msg);

      socket.emit("update_userlist", Object.keys(users));      

      console.log(msg + ' connected');
    }
  });

  socket.on('chat_message', function(msg){
    console.log(msg.username + "-" + msg.message);
    io.emit('chat_message', msg);
  });
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});