<template>
  <div id="app">
    <SideMenuComponent />
    <ChatComponent msg="Welcome to Your chat"/>
  </div>
</template>

<script>
import SideMenuComponent from './components/SideMenuComponent.vue'
import ChatComponent from './components/ChatComponent.vue'

export default {
  name: 'app',
  components: {
    SideMenuComponent,
    ChatComponent
  }
}
</script>

<style>
body {
  margin: 0;
}

#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  
  width: 100%;
  height: 100%;
}

.sidemenu {
  width: 20%;
  height: 100vh;
  background-color: #3862a2;
  float: left;
}

.chat {
  width: 80%;
  float: right;
}
</style>
