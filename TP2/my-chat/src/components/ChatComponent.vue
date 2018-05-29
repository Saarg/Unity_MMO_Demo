<template>
    <div class="chat">
        <h1>{{ msg }}</h1>

        <ul id="messages">
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
        props: {
            msg: String
        },
        data: function () {
            return {
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
            chat_message: function (val) {
                this.messages.push(val);
            },
            clear_chat: function () {
                this.messages = [];
            },
            user_join: function (data) {
                this.messages.push({ username: data.username, message: " joined " + data.room });
            },
            user_left: function (data) {
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

    form {
        background: #000;
        padding: 3px;
        position: fixed;
        bottom: 0;
        width: inherit;
    }

        form input {
            border: 0;
            padding: 10px;
            width: 90%;
            margin-right: .5%;
        }

        form button {
            width: 9%;
            background: rgb(130, 224, 255);
            border: none;
            padding: 10px;
        }

    #messages {
        list-style-type: none;
        margin: 0;
        padding: 0;
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
