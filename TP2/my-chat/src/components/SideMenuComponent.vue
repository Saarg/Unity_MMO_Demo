<template>
    <div class="sidemenu">
        <ul id="users">
            <li v-for="user in users" :key="user">
                {{ user }}
            </li>
        </ul>
    </div>
</template>

<script>
export default {
  name: 'SideMenuComponent',
  data: function () {
    return {
      users: []
    }
  },
  sockets:{
    user_login: function(val){
      this.users.push(val);
    },
    update_userlist: function(val){
      this.users = val;
    },
    user_logout: function(val) {
        this.users.splice(this.users.indexOf(val), 1);
    }
  },
  methods: {
    
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
    #users { 
        list-style-type: none;
        margin: 0;
        padding: 0; 
    }
    #users li { 
        padding: 5px 10px;
        color: white;
        text-align: left;
    }
    #users li:nth-child(odd) { 
        background: #EEE2;
    }
</style>
