import Vue from 'vue'
import App from './App.vue'
import VueSocketio from 'vue-socket.io';

Vue.use(VueSocketio, 'http://localhost:3000');

Vue.config.productionTip = false

new Vue({
  render: h => h(App)
}).$mount('#app')
