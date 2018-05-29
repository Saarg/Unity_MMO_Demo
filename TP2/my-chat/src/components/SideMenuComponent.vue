<template>
    <div class="sidemenu">
        <ul id="rooms">
            <li onmouseover="" style="cursor: pointer;" v-on:click="joinRoom(key)" v-for="(value, key) in rooms" :key="key">
                {{ key }}
                <ul id="users">
                    <li v-for="user in value" :key="user">
                        {{ user }}
                    </li>
                </ul>
            </li>
        </ul>
    </div>
</template>

<script>
    export default {
        name: 'SideMenuComponent',
        data: function () {
            return {
                curRoom: "general",
                rooms: {
                    general: [],
                    room01: [],
                    room02: [],
                    room03: []
                }
            }
        },
        sockets: {
            user_join: function (data) {
                console.log("user join called")

                if (this.rooms[data.room].indexOf(data.username) != -1)
                    return;
              
                if (!this.rooms[data.room])
                    this.rooms[data.room] = [];

                this.rooms[data.room].push(data.username);
                this.$forceUpdate();
            },
            user_left: function (data) {
                this.rooms[data.room].splice(this.rooms[data.room].indexOf(data.username), 1);
                this.$forceUpdate();
            }
        },
        methods: {
            joinRoom: function (room) {
                console.log("join room called")

                if (this.curRoom == room)
                    return;
                
                this.curRoom = room;
                this.$socket.emit('join_room', room);
            }
        }
    }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    #sidemenu {
        margin: 0;
        padding: 0;
        width: 100%;
        height: 100%;
        background-color: #3862a2;
        box-sizing: border-box;
    }

    #rooms, #users {
        list-style-type: none;
        margin: 0;
        padding: 0;
    }

        #rooms, #users li {
            padding: 5px 10px;
            color: white;
            text-align: left;
        }

            #rooms, #users li:nth-child(odd) {
                background: #EEE2;
            }
</style>
