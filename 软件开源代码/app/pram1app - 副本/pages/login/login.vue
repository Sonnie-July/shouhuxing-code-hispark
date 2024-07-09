<template>
  <view class="content">
    <view class="header">
      <image src="../../static/picture1/login.png"></image>
    </view>

    <view class="list"> 
      <view class="list-call">
        <image class="img" src="/static/login/yonghu.png"></image>
        <input class="sl-input" v-model="userName" type="text" maxlength="11" placeholder="输入用户名" />
      </view>
      <view class="list-call">
        <image class="img" src="/static/login/mima.png"></image>
        <input class="sl-input" v-model="password" type="text" maxlength="32" placeholder="输入密码" password="true" />
      </view>

	</view>
<!-- 移动端的主要问题是click会有300ms的延迟，为了减少这300ms的延迟，tap事件被很多框架（如zepto）封装，来减少这延迟问题， tap事件不是原生的，所以是封装的-->
		<view class="button-login" hover-class="button-hover" @tap="bindLogin()">
		<text>登 录</text>
		</view>
		<view class="agreenment">
		<navigator url="../../pages_volunteer/pages-login/forget" open-type="navigate">忘记密码</navigator>
		<text>|</text>
		<navigator url="../../pages_volunteer/pages-login/register" open-type="navigate">注册账户</navigator>
		</view>
  </view>
</template>

<script>
    import { mapState,mapMutations }from 'vuex'
  export default {
    data() {
      return {
        userName: '',
        password: '',
		info1:{identify:'',address:'',name:'',userName: '',picture:''}
      };
    },
    methods: {
      //   ...mapMutations(['login']),
      bindLogin() {
        uni.request({
          url: 'http://123.56.167.101:9999/checkLoginUser1',
      		  
          data: {
            userName: this.userName,
            password: this.password
          },
          method: 'POST',
      		  header: {
      		    'content-type': 'application/x-www-form-urlencoded'
      		  },
          dataType: 'json',
          success:res=>{
          	console.log(res)
          	if(res.data == 'no'){
      				uni.showToast({
      					title:'用户名或密码错误',
      					//icon:'none'
      				});
          	}else{
      				console.log("登录成功")
      				// var useinfo = res.data.split(',')
      
      				// this.info1.name = useinfo[0],
      				// this.info1.phone = useinfo[1],
      				// this.info1.identify = useinfo[2],
      				// this.info1.address  = useinfo[3],
      				// this.info1.picture = useinfo[4]
      				// console.log(typeof this.info1.picture)
      				// console.log(this.info1.picture)
      				// uni.setStorageSync('info1',this.info1)
      				// // that.toHome(that.phone);
      				
      				uni.reLaunch({
      					url: '/pages/index/index',
      				});
          	}
          }
        });
        
        
      
      },
	  // provider：供应商  手机号码
      toHome(provider) {
      	this.login(provider);
      	uni.reLaunch({
      		url: '../index/index',
      	});
      }
      
      
    }
  }
</script>

<style>
  .content {
      display: flex;
      flex-direction: column;
      justify-content: center;
  }
  .header {
      width: 160rpx;
      height: 160rpx;
      background: rgba(255, 255, 255, 1.0);
      box-shadow: 0rpx 12rpx 13rpx 0rpx #88d491;
      border-radius: 50%;
      margin: 20px auto 30px auto;
  }
  .header image {
      width: 161rpx;
      height: 161rpx;
      border-radius: 50%;
  }
  .list {
      display: flex;
      flex-direction: column;
  	  width: 100%;
  	  margin: auto;
      padding-top: 30px;
	  box-shadow: 0rpx -9px 13px 0rpx rgba(136, 212, 145,0.4);
	  border-radius:3em;
  }
  .list-call {
      display: flex;
      flex-direction: row;
      justify-content: space-between;
      align-items: center;
      height: 100rpx;
  	  width:80%;
  	  margin: auto;
  	  margin-bottom: 20px;
  	  background-color: #ffff;
  	  border-radius: 1em;
  	  box-shadow:0 5px 7px 0 rgba(0,0,0,0.15);
      border: 1px solid #e2e2e2;
  }
  .list-call .img{
        width: 40rpx;
        height: 40rpx;
		margin-left:20upx;
  }
  .list-call .sl-input {
      flex: 1;
      text-align: left;
      font-size: 32rpx;
      margin-left: 16rpx;
  }
  .button-login {
      color: #FFFFFF;
      font-size: 17px;
	  font-weight: 700;
      width: 470rpx;
      height: 100rpx;
      background:#88d491;
      border-radius: 50rpx;
      line-height: 100rpx;
      text-align: center;
      margin:auto;
  	margin-top: 30rpx;
      margin-bottom: 10px;
  }
  .button-hover {
      background: #d2e6ba;
  }
  .agreenment {
      display: flex;
      flex-direction: row;
      justify-content: center;
      align-items: center;
      font-size: 15x;
      margin-top: 20px;
  	margin-bottom: 20px;
      color: #52b13c;
      text-align: center;
      height: 40rpx;
      line-height: 40rpx;
  }
  .agreenment text {
      font-size: 24rpx;
      margin-left: 15rpx;
      margin-right: 15rpx;
  }
  

</style>
