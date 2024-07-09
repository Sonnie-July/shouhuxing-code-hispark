<template>
  <view class="content">
    <view class="header">
      <image src="../../static/picture1/login.png"></image>
    </view>
   

    <view class="list">
      <view class="list-call">
        <image class="img" src="/static/login/shouji.png"></image>
        <input class="sl-input" v-model="phoneNumber" type="number" maxlength="11" placeholder="手机号" />
      </view>
      <view class="list-call">
        <image class="img" src="/static/login/mima.png"></image>
        <input class="sl-input" v-model="password" type="text" maxlength="32" placeholder="登录密码" :password="!showPassword" />
        <image class="img" :src="showPassword?'/static/login/op.png':'/static/login/cl.png'" @tap="display"></image>
      </view>
      <view class="list-call">
        <image class="img" src="/static/login/yanzheng.png"></image>
        <input class="sl-input" v-model="code" type="text" maxlength="5" placeholder="验证码" />
        <view class="yzm" :class="{ yzms: second>0 }" @tap="getcode">{{yanzhengma}}</view>
      </view>


    </view>

    <view class="button-login" hover-class="button-hover" @tap="bindLogin">
      <text>注 册</text>
    </view>

    <view class="agreement">
      <image @tap="agreementSuccess" :src="agreement==true?'/static/login/ty1.png':'/static/login/ty0.png'"></image>
      <text @tap="agreementSuccess"> 同意</text>
      <navigator url="agreement?id=1" open-type="navigate">《软件用户协议》</navigator>
    </view>
  </view>
</template>

<script>
  var _this, js;
  export default {
    onLoad() {
      _this = this;

    },
    onUnload() {
      clearInterval(js)
      this.second = 0;
    },
    data() {
      return {
        phoneNumber: '',
        password: '',
        code: '',
        invitation: '',
        agreement: true,
        showPassword: false,
        second: 0
      };
    },
    computed: {
      yanzhengma() {
        if (this.second == 0) {
          return '获取验证码';
        } else {
          if (this.second < 10) {
            return '重新获取0' + this.second;
          } else {
            return '重新获取' + this.second;
          }
        }
      }
    },
    onUnload() {
      this.clear()
    },
    methods: {
      clear(){
        clearInterval(js)
        js = null
        this.second = 0
      },
      display() {
        this.showPassword = !this.showPassword
      },
      agreementSuccess() {
        this.agreement = !this.agreement;
      },
      getcode() {
        if (this.phoneNumber.length != 11) {
          uni.showToast({
            icon: 'none',
            title: '手机号不正确'
          });
          return;
        }
        if (this.second > 0) {
          return;
        }
        this.second = 60;
        //请求业务
        js = setInterval(function() {
          _this.second--;
          if (_this.second == 0) {
            _this.clear()
          }
        }, 1000)
        var that = this;
        console.log(that.phoneNumber);
        uni.request({
          url: "http://localhost:8082/jat/api/sendSms", 
          //url: 'http://192.168.31.225:8082/jat/api/sendSms',
          method:'POST',
          dataType:'json',
          data:{
          	phoneNumber: that.phoneNumber
          },
          header: {
          	'content-type': 'application/x-www-form-urlencoded'
          },
          success: (res) => {
            if (res.data.code != 200) {
              uni.showToast({
                title: res.data.msg,
                icon: 'none'
              });
            } else {
              uni.showToast({
                title: res.data.msg
              });
              js = setInterval(function() {
                _this.second--;
                if (_this.second == 0) {
                  _this.clear()
                }
              }, 1000)
            }
          },
          fail() {
            this.second == 0
          }
        });
      },
      bindLogin() {
        if (this.agreement == false) {
          uni.showToast({
            icon: 'none',
            title: '请先阅读《软件用户协议》'
          });
          return;
        }
        if (this.phoneNumber.length != 11) {
          uni.showToast({
            icon: 'none',
            title: '手机号不正确'
          });
          return;
        }
        if (this.password.length < 6) {
          uni.showToast({
            icon: 'none',
            title: '密码不正确'
          });
          return;
        }
        if (this.code.length != 5) {
          uni.showToast({
            icon: 'none',
            title: '验证码不正确'
          });
          return;
        }
        uni.request({
          url: 'http://localhost:8082/jat/api/adduser',
          data: {
            phoneNumber: this.phoneNumber,
            password: this.password,
            code: this.code
          },
          method: 'POST',
          dataType: 'json',
          header: {
          	'Content-Type': 'application/x-www-form-urlencoded'
          },
          success: (res) => {
            if (res.data.success == true) {
              uni.navigateTo({
              	url:'login'
              });
            } else {
              uni.showToast({
                title: res.data.msg
              });
              setTimeout(function() {
                uni.navigateBack();
              }, 1500)
            }
          }
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
    width: 161rpx;
    height: 161rpx;
    background: rgba(255, 255, 255, 1.0);
    box-shadow: 0rpx 12rpx 13rpx 0rpx #88d491;
    border-radius: 50%;
    border-radius: 50%;
    margin: 20px auto 10px auto;
  }

  .header image {
    width: 161rpx;
    height: 161rpx;
    border-radius: 50%;
  }

  .list {
    display: flex;
    flex-direction: column;
    padding-top: 50rpx;
  }

  .list-call {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
    height: 100rpx;
    width:80%;
	color: #333333;
	margin: auto;
	margin-bottom: 20px;
	background-color: #ffff;
	border-radius: 1em;
	box-shadow:0 5px 7px 0 rgba(0,0,0,0.15);
	border: 1px solid #e2e2e2;
  }

  .list-call .iconfont {
    width: 40rpx;
    height: 40rpx;
	font-size: 19px;
	color:green;
	margin-left:20rpx;
  }

  .list-call .img {
    width: 45rpx;
    height: 45rpx;
	margin-left: 20upx;
  }
  
  .list-call .sl-input {
    flex: 1;
    text-align: left;
    font-size: 32rpx;
    margin-left: 16rpx;
  }

  .yzm {
    color:#1b9c35;
    font-size: 24rpx;
    line-height: 64rpx;
    padding-left: 10rpx;
    padding-right: 10rpx;
    width: auto;
    height: 64rpx;
    border: 1rpx solid #1b9c35;
    border-radius: 50rpx;
  }

  .yzms {
    color: #999999 !important;
    border: 1rpx solid #999999;
  }

  .button-login {
    color: #FFFFFF;
    font-size: 17px;
    font-weight: 700;
    width: 470rpx;
    height: 100rpx;
    line-height: 100rpx;
    background: #88d491;
    border-radius: 50rpx;
    text-align: center;
    margin-left: auto;
    margin-right: auto;
    margin-top: 10rpx;
  }

  .button-hover {
    background: #d2e6ba;
  }

  .agreement {
    display: flex;
    flex-direction: row;
    justify-content: center;
    align-items: center;
    font-size: 30rpx;
    margin-top: 80rpx;
    color: black;
    text-align: center;
    height: 40rpx;
    line-height: 40rpx;
  }

  .agreement image {
    width: 40rpx;
    height: 40rpx;
  }

</style>
