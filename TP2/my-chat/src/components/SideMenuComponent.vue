<template>
    <div class="sidemenu">
        <ul id="rooms">
            <li v-on:click="joinRoom(key)" v-for="(value, key) in rooms" :key="key">
                {{ key }}
                <ul id="users">
                    <li v-for="user in value.users" :key="user">
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
        rooms: {}
    }
  },
  sockets:{
    update_userlist: function(data){
        console.log(data);
        
        this.rooms[data.name] = data;
        this.$forceUpdate();
    }
  },
  methods: {
    joinRoom: function(room){
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
