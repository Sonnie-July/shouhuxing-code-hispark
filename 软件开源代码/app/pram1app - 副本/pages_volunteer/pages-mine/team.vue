<template>
	<view class="content">
		<view class="cmd-cell-item">
			<view class="cmd-cell-item-body">
				<view class="cmd-cell-item-content">姓名</view>
					<input v-model="name" value="" placeholder="请填写姓名" />
			</view>
		</view>
		<view class="cmd-cell-item">
			<view class="cmd-cell-item-body">
				<view class="cmd-cell-item-content">手机号</view>
					<input  v-model="phone" type="number" placeholder="请填写手机号码"  />
			</view>
		</view>
		<view class="cmd-cell-item">
			<view class="cmd-cell-item-body">
				<view class="cmd-cell-item-content">身份证</view>
					<input :style="{backgroundColor: isCn ? '#EE6A50' : ''}" v-model="idNum" placeholder="请填写身份证号" type="text" />
			</view>
		</view>
		<view class="cmd-cell-item">
			<view class="cmd-cell-item-body">
				<view class="cmd-cell-item-content">详细住址</view>
					<input v-model="address" value="" placeholder="请填写详细地址" />
			</view>
		</view>
		<view class="cmd-cell-item">
			
		</view>
		
		<br>
		<view class="notice" v-if='sums'>
			<view class="icon">
				<image mode="aspectFit" src="../../static/picture1/lb2.jpg"></image>
			</view>
			<view class="text">目前已预约：{{sum}}  总预约：1000</view>
		</view>
		<button class="button-login"  @click="submit()"> 提交</button>
		
	</view>
</template>

<script>
	export default {
	data() {
		return {
			// 姓名
			name:'',
			//身份证
			idNum:'',
			//手机号码
			phone:'',
			//地址
			address:'',
			// 检测点
			candidates: ['计算机学院', '东餐厅'],
			// 选择的检测点
			jiancedian: '',
			// 预约时间段
			array:['---请选择---','7:00-8:00','8:00-9:00','09:00 -10:00 ','10:00-11:00','11:00-12:00','12:00-13:00','13:00-14:00','14:00-15:00','16:00-17:00'],
			// 选择的预约时间
			time:'',
			// 隐藏预约人数/总人数
			sums:false,
			// 已预约数
			sum:'',
			// 索引
			index:0
		}
	},
	methods: {
		// 测试
		test(){
			console.log(this.phone)
			console.log(this.name)
			console.log(this.address)
			console.log(this.idNum)
		},
		//下拉框
		bindPickerChange: function(e) {		//改变的事件名
			//console.log('picker发送选择改变，携带值为', e.target.value)   
			// 用于输出改变索引值
			this.index = e.target.value			//将数组改变索引赋给定义的index变量
			// 获取预约时间
			this.time=this.array[this.index]		//将array【改变索引】的值赋给定义的jg变量
			// 设置已预约时间
			this.sum=Math.floor(Math.random()*1000) 
			this.sums=true
			console.log(this.time)
		},
		submit(){
			let that = this;
			// if (!this.name) {
			// 	uni.showModal({
			// 		content: '请填写人员姓名',
			// 		showCancel: false
			// 	})
			// 	return false
			// }
			// if (!this.phone) {
			// 	uni.showModal({
			// 		content: '请填写手机号码',
			// 		showCancel: false
			// 	})
			// 	return false
			// }
			// this.test(),
			uni.request({
					url: 'http://127.0.0.1:8000/wxx/yuyue/',
					data:{
						name: that.name,
						idNum: that.idNum,
						phone: that.phone,
						address: that.address,
						time:that.time,
						jiancedian:this.jiancedian
					},
					method: 'POST',
					responseType: "arraybuffer",
					// header: {
					// 	'Content-Type': 'application/json;charset=utf-8'
					// },
					dataType: 'json',
					success:res=>{
						console.log(res.data)
						this.qrcode=res.data
					}
				}),
			
				uni.navigateTo({
				  url: 'jiance'
				})
			},
		},
		
	}
	
</script>

<style lang="scss" scoped>
	.sex{
		margin: 0 60px;
	}
	.a{
		margin: 0 10px;
	}
	.btn-logout {
	  margin-top: 100upx;
	  width: 80%;
	  border-radius: 50upx;
	  font-size: 16px;
	  color: #ffffff;
	  background: #ffb7dd;
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
	.btn-logout-hover {
	  background: #ffb7dd;
	}
	.cmd-cell-item {
		position: relative;
	}
	
	.cmd-cell-item.no-border .cmd-cell-item-body::before {
		display: none;
	}
	.cmd-cell-item-body {
		position: relative;
		display: flex;
		align-items: center;
		/* justify-content: space-between; */
		min-height: 100upx;
		padding-top: 20upx;
		padding-bottom: 20upx;
		margin-left: 20upx;
		margin-right: 20upx;
		box-sizing: border-box;
	}
	.cmd-cell-item-content {
		/* 例如 姓名与输入框之间的距离 */
		flex: 0.5 0.5 0%;
		
		
		color: #111A34;
		font-size: 32upx;
		line-height: 1.2;
	}
	
	.cmd-cell-item.is-disabled .cmd-cell-item-content,
	.cmd-cell-item.is-disabled .cmd-cell-item-title,
	.cmd-cell-item.is-disabled .cmd-cell-item-brief,
	.cmd-cell-item.is-disabled .cmd-cell-item-left,
	.cmd-cell-item.is-disabled .cmd-cell-item-children {
		color: #C5CAD5;
	}
	
	.cmd-cell-item-hover {
		background: transparent;
	}
	.notice {
		display: flex;
		background-color: #ffb7dd;
		height: 100upx;
		align-items: center;
		padding: 10upx 20upx;
	
		.icon {
			display: flex;
			justify-content: center;
			align-items: center;
	
			image {
				width: 30upx;
				height: 30upx;
			}
		}
	
		.text {
			flex: 1;
			margin-left: 10upx;
			color: #ff8cc4;
			font-size: 26upx;
			padding:0 20upx;
		}
	
		.option {
			font-size: 26upx;
			color: rgb(153, 153, 153);
		}
	}
</style>
