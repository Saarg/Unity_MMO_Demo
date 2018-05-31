<template>
    <div class="sidemenu">
        <ul id="rooms">
            <li v-on:click="joinRoom(key)" v-for="(value, key) in rooms" :key="key" v-bind:class="{ current: curRoom==key, empty: value.length == 0 }">
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
            user_is_in_room: function (data) {
                this.user_join(data);
            },
            user_join: function (data) {
                this.user_join(data);
            },
            user_left: function (data) {
                this.rooms[data.room].splice(this.rooms[data.room].indexOf(data.username), 1);
                this.$forceUpdate();
            }
        },
        methods: {
            user_join: function (data) {
                if (this.rooms[data.room].indexOf(data.username) != -1)
                    return;
              
                if (!this.rooms[data.room])
                    this.rooms[data.room] = [];

                this.rooms[data.room].push(data.username);
                this.$forceUpdate();
            },
            joinRoom: function (room) {
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
        padding: 0px;
        padding-bottom: 10px;
        margin: 0px;
        color: white;
        text-align: left;
    }

    #rooms li {
        background-color: #3862a2;
        padding-top: 10px;      
        padding-left: 10px;       
        cursor: pointer;   
    }

    #rooms .empty {
        padding-bottom: 3px;
    }

    #rooms li:hover {
        background-color: #26497e;
    }

    #users {
        margin-top: 10px;       
    }

    #users li {
        margin-left: -10px;
        padding-left: 20px;        
    }

    #users li:nth-child(even) {
        background-color: #345b97;
    } 

    #users li:nth-child(odd)  {
        background-color: #3966ab;
    }

    #rooms .current {
        background-color: #224780;
    }
</style>
