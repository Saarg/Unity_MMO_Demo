<template>
    <div class="chat">
        <h1>{{ room }}</h1>

        <ul id="messages" v-chat-scroll>
            <li v-for="message in messages" :key="message.username">
                {{ message.username }} - {{ message.message }}
            </li>
        </ul>

        <form action="">
            <input id="m" autocomplete="off" type="text" v-model="curMessage" />
            <button v-on:click="sendMessage">Send</button>
        </form>
    </div>
</template>

<script>
    export default {
        name: 'ChatComponent',
        data: function () {
            return {
                room: "general",
                username: "",
                messages: [],
                curMessage: ""
            }
        },
        sockets: {
            connect: function () {
                console.log('socket connected')
            },
            login_refused: function () {
                this.sendUsername();
            },
            chat_history: function (data) {
                this.messages = data;
            },
            chat_message: function (val) {
                this.messages.push(val);
            },
            clear_chat: function () {
                this.messages = [];
            },
            user_join: function (data) {
                if (data.username == this.username) {
                    this.room = data.room;
                    return;
                }

                if (data.room != this.room)
                    return;

                this.messages.push({ username: data.username, message: " joined " + data.room });
            },
            user_left: function (data) {
                if (data.room != this.room)
                    return;
                
                this.messages.push({ username: data.username, message: " left " + data.room });
            }
        },
        methods: {
            sendUsername: function () {
                do {
                    this.username = prompt("Please enter your name");
                } while (this.username == null || this.username == "");

                this.$socket.emit('user_login', this.username);
            },
            sendMessage: function (event) {
                event.preventDefault();
                // $socket is socket.io-client instance
                this.$socket.emit('chat_message', { username: this.username, message: this.curMessage });
                this.curMessage = "";
            }
        },
        created: function () {
            this.sendUsername();
        }
    }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
    }

    .chat {
        height: 100vh;
        overflow-x: hidden;
    }

    h1 {
        background: #f2f2f2; 
        height: 50px;      
        width: 100%; 
    }

    form {
        background: #3862a2;
        padding: 5px 3px 5px 0px;
        position: fixed;
        bottom: 0;
        width: inherit;
        height: 50px;
    }

    form input {
        border: 0;
        padding: 10px;
        width: 90%;
        margin-right: .5%;
        border-radius: 3px;
    }

    form button {
        width: 9%;
        background: rgb(82, 125, 191);
        border-radius: 10%;
        padding: 10px;
        color: white;
        border: none;
    }

    form button:hover {
        background: rgb(34, 71, 128);
    }

    #messages {
        list-style-type: none;
        margin: 0;
        padding: 0;    
        width: 100%;
        position: fixed;
        bottom: 50px;
        top: 50px;
        overflow-y: scroll;
    }

    #messages li {
        padding: 5px 10px;
        width: 100%;
        text-align: left;
    }

    #messages li:nth-child(odd) {
        background: #eee;
    }
</style>
