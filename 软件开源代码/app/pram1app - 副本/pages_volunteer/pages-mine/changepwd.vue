<template>
	<view class="content">
		<form @submit="sub">
			<view class="list">
			<view class="list-item">
				<text>旧密码： </text>
				<input v-model="passward" value="" placeholder="请输入旧密码" />
			</view>
			<view class="list-item">
				<text>新密码： </text>
				<input v-model="newpassward" value="" placeholder="请输入新密码" />
			</view>
			</view>
			<view >
				<button form-type="submit" @tap="sub()" class="button-login">确定</button>
			</view>
		</form>
	</view>
</template>

<script>
	export default {
		data() {
			return {
				name:'',
				passward: '',
				newpassward: ''
			}
			},
		methods: {
			onLoad() {
				var data = uni.getStorageSync('info1')
				this.name = data.name
				},
			sub: function(e) {
				var that = this;
				uni.request({
					url: 'http://127.0.0.1:8000/wxx/changepassward/',
					data: {
						name:that.name,
						passward: that.passward,
						newpassward: that.newpassward
					},
					method: 'POST',
					header: {
						'Content-Type': 'application/x-www-form-urlencoded'
					},
					success:res=>{
						if(res.data == "Success"){
							console.log("修改成功")
							uni.showToast({
								title:'修改成功',
								icon:'none'
							});
						}else{
							console.log(res.data);
							uni.showToast({
								title:'提交失败',
								icon:'none'
							});
						}
					}
				})
			}											
			}
		}
	
	
</script>

<style>
	page {
			background: #f0f0f0;
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
	  margin-top: 100rpx;
	}
		.content {
			display: flex;
			flex-direction: column;
			align-items: center;
			justify-content: center;
			margin-bottom: 50rpx;
		}
	
		.list {
			display: flex;
			flex-direction: column;
			width: 680rpx;
			margin: auto;
			margin-top: 20rpx;
			background: #ffffff;
			border-radius: 20rpx;
			padding-bottom: 20rpx;
		}
	
		.list-title {
			width: 580rpx;
			padding: 20rpx;
			display: flex;
			font-size: 40rpx;
			font-weight: bold;
			border-bottom: #f0f0f0 1rpx solid;
			margin: auto;
			margin-bottom: 20rpx;
	
		}
	
		.list-item {
			width: 580rpx;
			padding: 20rpx;
			display: flex;
			margin: auto;
			font-size: 30rpx;
			align-items: center;
		}
	
		.list-item>text {
			white-space: nowrap;
		}
	
		.list-item input {
			background: #f0f0f0;
			border-radius: 10rpx;
			padding-left: 20rpx;
			padding-right: 20rpx;
			width: 600rpx;
			height: 100rpx;
			color: #999999;
			font-size: 30rpx;
		}
	
		.list-item slider {
			width: 300rpx;
		}
	
		.list-item picker {
			margin-right: 10px;
		}
	
		.list-item picker button {
			vertical-align: middle;
		}
	
		.list-item label {
			display: inline-block;
			margin: 5px 0px;
		}
	
		.list-item label text {
			margin-right: 10px;
		}
	
		.list-item textarea {
			border-radius: 5px;
			background-color: #F8F8F8;
			padding: 10px;
		}
	
		.xiala {
			display: flex;
			background: #f0f0f0;
			padding: 10rpx;
			border-radius: 10rpx;
			color: #999999;
			font-size: 30rpx;
	
		}
	
		.xiala image {
			display: flex;
			padding-top: 5rpx;
			width: 30rpx;
			height: 30rpx;
			margin-left: 10rpx;
	
		}
	
		.divider {
			content: '';
			width: 580rpx;
			margin: 5px auto;
			height: 1px;
			background-color: #DDDDDD;
		}
	
		.color-red {
			color: #FF3333;
		}
</style>
