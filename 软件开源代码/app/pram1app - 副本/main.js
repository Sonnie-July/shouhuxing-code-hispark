import App from './App'

// #ifndef VUE3
import Vue from 'vue'
Vue.config.productionTip = false
App.mpType = 'app'
const app = new Vue({
    ...App
})
app.$mount()
// #endif

// #ifdef VUE3
import { createSSRApp } from 'vue'
export function createApp() {
  const app = createSSRApp(App)
  return {
    app
  }
}
// #endif

Vue.prototype.ScanAudio = function() {
 
    var music = null;
 
    music = uni.createInnerAudioContext(); //创建播放器对象
 
    music.src = "static/music/0.aac";
 
    music.play(); //执行执行播放
 
    music.onEnded(() => {
 
        //音频播放结束
 
        music = null;
 
    });}
import appPush from '@/js_sdk/levine-APPPush/levine-APPPush/plugins/APPPush/index.js'
Vue.use(appPush)