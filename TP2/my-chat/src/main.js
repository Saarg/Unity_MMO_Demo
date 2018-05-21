import Vue from 'vue'
import App from './App.vue'
import VueSocketio from 'vue-socket.io';

Vue.use(VueSocketio, 'http://192.168.2.57:3000');

Vue.config.productionTip = false

new Vue({
  render: h => h(App)
}).$mount('#app')
