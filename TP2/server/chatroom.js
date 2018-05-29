function Chatroom(io, name) {
    this.io = io;
    this.name = name;
    this.users = {};
    this.messagesHistory = [];
}

Chatroom.prototype.join = function join(socket) {
    this.users[socket.username] = socket;

    this.io.of('/').adapter.remoteJoin(socket.id, this.name, (err) => {
        if (err) { /* unknown id */ }
        this.io.emit("user_join", { room: this.name, username: socket.username });

        this.io.of('/').adapter.clients([this.name], (error, clients) => {
            if (error) throw error;

            clients.forEach(client => {
                if (client != socket.id) {
                    socket.emit("user_is_in_room", { room: this.name, username: this.io.sockets.connected[client].username });
                }
                else {
                    socket.emit("chat_history", this.messagesHistory);
                }
            });
        });

        console.log(socket.username + " joined " + this.name);
    });
}

Chatroom.prototype.leave = function leave(socket) {
    delete this.users[socket.username];

    this.io.of('/').adapter.remoteLeave(socket.id, this.name, (err) => {
        if (err) { /* unknown id */ }
        this.io.emit("user_left", { room: this.name, username: socket.username });
        console.log(socket.username + " left " + this.name);
    });
}

Chatroom.prototype.disconnect = function disconnect(socket) {
    delete this.users[socket.username];

    this.io.of('/').adapter.remoteLeave(socket.id, this.name, (err) => {
        if (err) { /* unknown id */ }
        console.log(socket.username + " left " + this.name);
    });
}

Chatroom.prototype.findUser = function findUser(username) {
    return this.users[username];
}

Chatroom.prototype.emit = function emit(msg_id, msg) {
    this.io.in(this.name).emit(msg_id, msg);
    this.messagesHistory.push(msg);
}

module.exports = Chatroom;