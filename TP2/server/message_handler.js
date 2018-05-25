module.exports = (io) => {
    const ChatRoom = require('./chatroom');

    var chatrooms = {
        general: new ChatRoom(io, "general"),
        room01: new ChatRoom(io, "room01"),
        room02: new ChatRoom(io, "room02"),
        room03: new ChatRoom(io, "room03")
    }

    io.on('connection', function (socket) {
        socket.on('disconnect', function () {
            for (let room in chatrooms) {
                chatrooms[room].disconnect(socket);
            }
        });

        socket.on('user_login', function (username) {
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

        socket.on('join_room', function (msg) {
            io.of('/').adapter.clientRooms(socket.id, (err, rooms) => {
                rooms.forEach(r => {
                    chatrooms[r].leave(socket);
                });
            });

            chatrooms[msg].join(socket);
        });

        socket.on('chat_message', function (msg) {
            for (let r in socket.rooms) {
                chatrooms[socket.rooms[r]].emit('chat_message', msg);
            }
        });
    });
};
