<template>
	<div>
	<div>
	<view>
		<view class="page-body">
			<view class="btn-area">
				<button class="btn-logout1">
					<image class="pic" src="../../static/picture1/tem.png"></image>
					<span class="te1">温度</span>
					<span class="te2">{{ sensorData.temperature }}</span>
				</button>
				<button class="btn-logout2">
					<image class="pic" src="../../static/picture1/wet.png"></image>
					<span class="te1">湿度</span>
					<span class="te2">{{ sensorData.humidity }}</span>
				</button>
				<button class="btn-logout3">
					<image class="pic" src="../../static/picture1/guangzhao.png"></image>
					<span class="te1">光照强度</span>
					<span class="te2">{{ sensorData.illumination }}</span>
				</button>
			</view>
		</view>
	</view>
	</div>
	</div>
</template>
<script>
export default {
		  data() {
		    return {
		      sensorData: {
		        humidity: '5',
		        humaninfrared: '5',
		        bedWetting: '2',
		        renti: '是',
		        illumination: '5',
		        temperature: '25',
		        niaChung: '是'
		      }
		    };
		  },
		  created() {
		      // 当组件创建时启动定时器
		        setInterval(() => {
		  			if (this.sensorData.illumination >=300 && !this.isPopupVisible) {
		  				let params = {
		  					inApp: true,
		  					voice: true,
		  					vibration: true,
		  					messageType: '消息分类',
		  					messageTitle: '光照强度过高',
		  					messageContent: '请打开遮阳棚',
		  					messageImage: '/static/picture1/attention.png',
		  					};
		  				this.$appPush(params);
		  				this.isPopupVisible = true; // 弹窗显示状态为true
		  				setTimeout(() => {
		  					this.isPopupVisible = false; // 延时后关闭弹窗
		  				}, 10000);
		  			}
					else if (this.sensorData.temperature >=30 && !this.isPopupVisible) {
		  				let params = {
		  					inApp: true,
		  					voice: true,
		  					vibration: true,
		  					messageType: '消息分类',
		  					messageTitle: '环境温度过高',
		  					messageContent: '宝宝需要照顾',
		  					messageImage: '/static/picture1/attention.png',
		  					};
		  				this.$appPush(params);
		  				this.isPopupVisible = true; // 弹窗显示状态为true
		  				setTimeout(() => {
		  					this.isPopupVisible = false; // 延时后关闭弹窗
		  				}, 10000);
		  			}
		        }, 5000);
		    },
		  methods: {
		    getSensorData() {
		      uni.request({
		        url: 'http://123.56.167.101:9999/document_flush1',
		        success: (res) => {
		          console.log('Response:', res.data);
		          this.sensorData = res.data;
		        },
		        fail: (err) => {
		          console.error('Error fetching sensor data:', err);
		        },
		        complete: () => {
		          // 请求完成后设置下一次请求的定时器
		          setTimeout(this.getSensorData, 3000); // 每隔3秒发送一次请求
		        }
		      });
		    }
		  },
		  mounted() {
		    this.getSensorData();
		  }
		}
</script>

<style>
	.te1{
		margin-left: 20px;
		font-size: 30px;
		color:#344b4f;
		font-weight: 600;
	}
	.te2{
		font-size: 25px;
		color:#344b4f;
		position: absolute;
		top: 50px;
		left: 135px;
		font-weight: 600;
	}
	.pic{
		width: 70px;
		height: 70px;
		float: left;
		margin-top:25px;
		margin-left: 10px;
		padding:10px;
	}
.uni-picker-tips {
	font-size: 12px;
	color: #666;
	margin-bottom: 15px;
	padding: 0 15px;
	/* text-align: right; */
}
.btn-logout1 {
		  margin-top: 20px;
		  width: 90%;
		  height: 140px;
		  color: #fff;
		  background:linear-gradient(to bottom right, #afe5c2,#f1ccb8 );
		   text-align: left;
		  border-radius: 2em;
		  box-shadow: 3px 10px 15px 1px rgb(0,0,0,0.1);
		}
	.btn-logout2 {
		  margin-top: 20px;
		  width: 90%;
		  height: 140px;
		  color: #fff;
		  background:linear-gradient(to bottom right, #afe5c2,#f1ccb8 );
		   text-align: left;
		  border-radius: 2em;
		  box-shadow: 3px 10px 15px 1px rgb(0,0,0,0.1);
		}
		.btn-logout3 {
			 margin-top: 20px;
			 width: 90%;
			 height: 140px;
			 color: #fff;
			 background:linear-gradient(to bottom right, #afe5c2,#f1ccb8 );
			  text-align: left;
			 border-radius: 2em;
			 box-shadow: 3px 10px 15px 1px rgb(0,0,0,0.1);
			}
			.page-body{
				position: absolute;
				background-color: #f1f1f1;
				width: 100%;
				height: 100vh;
			}
</style>
