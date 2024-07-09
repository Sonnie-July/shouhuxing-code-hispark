<template>
	<view>
		<view class="profile" @tap="toProfile">
		<!--<hx-navbar ref="hxnb" :config="config">
			<block slot="left">
				<view class="a1">
					<image src="../../static/bar/zz1.png" class="a3" @click="set"></image>
				</view>
			</block>
			<block slot="center">
				<view class="a4">
					我的
				</view>
			</block>
		</hx-navbar>-->
			<view class="a2">
				<view class="a5">
					<view class="a6">用户名</view>
					<view class="a7">立即登录</view>
				</view>
				<image src="../../static/picture1/toux.png" class="a8_1"></image>
			</view>
		</view>
		<view class="bottom">
		<view class="person-list">
		  <cmd-cell-item title="个人信息" slot-left arrow @tap="gonavigate1">
		    <cmd-icon type="bullet-list" size="24" color="green"></cmd-icon>
		  </cmd-cell-item>
		  <cmd-cell-item title="历史数据" slot-left arrow @tap="gonavigate2">
		    <cmd-icon type="message" size="24" color="green"></cmd-icon>
		  </cmd-cell-item>
		  <cmd-cell-item title="修改密码" slot-left arrow  @tap="gonavigate3">
		    <cmd-icon type="settings" size="24" color="green"></cmd-icon>
		  </cmd-cell-item>
		  <cmd-cell-item title="注意事项"slot-left arrow @tap="gonavigate4">
		    <cmd-icon type="alert-circle" size="24" color="green"></cmd-icon>
		  </cmd-cell-item>
		  <cmd-cell-item title="关于我们"slot-left arrow @tap="gonavigate5">
		    <cmd-icon type="share" size="24" color="green"></cmd-icon>
		  </cmd-cell-item>
		</view>
		</view>
	</view>
</template>
	
<script>
	import cmdAvatar from "@/components/cmd-avatar/cmd-avatar.vue"
	import cmdIcon from "@/components/cmd-icon/cmd-icon.vue"
	import cmdCellItem from "@/components/cmd-cell-item/cmd-cell-item.vue"
	export default {
		components: { cmdAvatar, cmdCellItem, cmdIcon},
		data() {
			return {
				name:'',
				address:'',
				picture:'',
				pictureUrl:''
			}
		},
		methods: {
			getImg(img){
				console.log(img)
				return "../../static/" + img
			},
			onLoad(){
				var data = uni.getStorageSync('info1')
				console.log(data.picture)
				this.name = data.name
				this.address = data.address
				this.picture = data.picture
				
				this.pictureUrl = "../../static/" + this.picture
				console.log(this.pictureUrl)
			},
			gonavigate1(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-mine/info'
				});
			
			},
			gonavigate2(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-mine/tableDemo'
				});
			
			},
			gonavigate3(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-mine/changepwd'
				});
			
			},
			gonavigate4(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-mine/check'
				});
			
			},
			gonavigate5(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-mine/team'
				});
			
			}
		}
	}
</script>

<style>
  /*.person-head {
    display: flex;
    flex-direction: row;
    align-items: center;
    height: 160px;
    padding-left: 20px;
    background: #ff8cc4;
  }

  .person-head-box {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: flex-start;
    margin-left: 10px;
  }

  .person-head-nickname {
    font-size: 18px;
    font-weight: 500;
    color: #fff;
  }

  .person-head-username {
    font-size: 14px;
    font-weight: 500;
    color: #fff;
  }*/
  .person-list {
	width: 90%;
	margin:auto;
	margin-top:190px;
	border-radius: 1em;
    line-height: 0;
	background-color: #fff;
  }
  .bottom{
	  background-color:#f1f1f1;
	  position: absolute;
	  top:0px;
	  width: 100%;
	  height: 100vh;
  }
  .a1 {
  	padding-left: 35upx;
  }
  .a2 {
  	background-image: linear-gradient(#88d491,#FFFFFF);
  	height: 170px;
	width: 100%;
	background-color: pink ;
  	display: flex;
  	flex-direction: row;
  	align-items: center;
  	justify-content: space-between;
	position: absolute;
	z-index: 2;
  }
  .a3{
  	width: 50upx;height: 50upx;
  }
  .a4{
  	margin-left: 220upx;font-size: 34upx;font-weight: 700;
  }
  .a5{
  	display: flex;flex-direction: column;
	position: absolute;
	left:50%;
	width: 100px;
	bottom: 10rpx;
	margin-left:-50px;
  }
  .a6{
  	font-size: 40upx;font-weight: 700;margin: auto;
  }
  .a7{
  	font-size: 24upx;
	color: darkgray;
	margin: 8upx auto;
	width: 60%;
	text-align: center;
	border-radius: 2em;
	border:solid 2px green;
	background-color: #fff;
  }	
  .a8_1{
		height: 70px;width:70px;border-radius: 100%;
		margin:-20px auto 0 auto;
	}
</style>
