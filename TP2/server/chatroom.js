function Chatroom(io, name) {
    this.io = io;
    this.name = name;
    this.users = {};
}

Chatroom.prototype.join = function join(socket) {
    this.users[socket.username] = socket;
    socket.join(this.name);
    this.updateUserList();
    socket.emit("clear_chat");
}

Chatroom.prototype.leave =function leave(socket) {
    if (!this.users[socket.username])
        return;
    
    delete this.users[socket.username];

    this.updateUserList();
    console.log(socket.username + " left " + this.name);        
    socket.leave(this.name);
}

Chatroom.prototype.disconect =function disconect(socket) {
    if (!this.users[socket.username])
        return;

    delete this.users[socket.username];

    this.updateUserList();
    console.log(socket.username + " left " + this.name);
}

Chatroom.prototype.updateUserList =function updateUserList(socket) {
    if (socket)
        socket.emit("update_userlist", { name: this.name, users: Object.keys(this.users) }); 
    else
        this.io.emit("update_userlist", { name: this.name, users: Object.keys(this.users) }); 
}

Chatroom.prototype.findUser =function findUser(username) {
    return this.users[username];
}

Chatroom.prototype.emit =function emit(msg_id, msg) {
    this.io.in(this.name).emit(msg_id, msg);
}

module.exports = Chatroom;