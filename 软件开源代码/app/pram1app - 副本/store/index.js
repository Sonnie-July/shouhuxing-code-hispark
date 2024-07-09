import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

const store = new Vuex.Store({
	state: {
		/**
		 * 是否需要强制登录
		 */
		forcedLogin: false,
		hasLogin: false,
		phone:'',
		userInfo: {}
	},
	mutations: {
		// 添加用户
		adduser(state,provider){
			state.hasLogin = true;
			state.userInfo = provider;
			uni.setStorage({
				key: 'userInfo',
				data: provider
			})
			console.log(state.userInfo);
		},
		// 登录
		login(state, phone) {
			state.phone = phone;
			state.hasLogin = true;
			// 将数据存储在本地缓存中指定的 key 中，会覆盖掉原来该 key 对应的内容，这是一个异步接口。
			uni.setStorage({
				key: 'phone',
				data: phone
			})
			console.log(state.phone);
		},
		// 退出
		logout(state) {
			state.userInfo = {};
			state.phone = '';
			state.hasLogin = false;
			// 从本地缓存中异步移除指定 key。
			uni.removeStorage({
				key: 'userInfo',
				key: 'phone',
			})
		}
	}
})

export default store
