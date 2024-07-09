<template>
	<view class="all">
		<view class="baby">
			<image class="baby1" src="../../static/picture1/baby.png"></image>
			<text class="baby2">{{ sensorData.emoji }}</text>
		</view>
		<view class="car">
			<image class="car1" src="../../static/picture1/babycar.png"></image>
			<view class="car2">{{ direct }}</view>
		</view>
		<view class="san1">
		<view class="top">
			<button @click="forw"></button>
		</view>
		</view>
		<view class="stop">
			<button @click="stop"></button>
		</view>
		<view class="san2">
		<view class="last">
			<button @click="back"></button>
		</view>
		</view>
		<view class="san3">
		<view class="right">
			<button @click="right"></button>
		</view>
		</view>
		<view class="san4">
		<view class="left">
			<button @click="left"></button>
		</view>
		</view>
		<view class="sleepy">
			<view class="sleep">
				<button @click="sleep">哄睡</button>
			</view>
			<view class="sleep">
				<button @click="sleep">哄睡</button>
			</view>
		</view>
		<view class="bottom">
			<view  class="socket">
				<button @click="btn">连 接</button>
			</view>
			<view  class="socket">
				<button @click="close">断 开</button>
			</view>
		</view>
	</view>
</template>

<script>
	const TCPSocket = uni.requireNativePlugin('Aimer-TCPPlugin');
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
		        niaChung: '否',
		      },
			  direct:'',
			  flag:false,
		    }
		},
		onLoad(options) {
			this.getSensorData();
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
			},
			btn(){
				TCPSocket.connect(
				    {
				        //charsetname:'GBK',//可不选,默认UTF-8,针对服务端数据的字符集格式转化
				        channel:'1',//可选 1~20
				        ip: '192.168.43.21',
				        port: '6666'
				    },
				    result => {
				                    /**
				                     * status : 0 连接成功
				                     * status : 1 断开连接
				                     * receivedMsg : 服务器返回字符串(普通的字符串交互)
				                     * receivedHexMsg : 服务器返回字节数组(单片机、智能家居等硬件数据交互)
				                     */
				                    if (result.status == '0') {
				                        //TCP连接成功
				                        console.log('TCP连接成功');
										this.flag=true;
				                    } else if (result.status == '1') {
				                        //TCP断开连接
				                        console.log('TCP断开连接');
										this.flag=false;
				                    }
				                    if (result.receivedMsg) {
				                        //服务器返回字符串
				                        console.log(result.receivedMsg);
				                    }
				                    if (result.receivedHexMsg) {
				                        //硬件服务器返回16进制数据
				                        console.log(result.receivedHexMsg);
				                        let msg=result.receivedHexMsg;
				                        let sum = msg.length / 2;
				                        let arr = [];
				                        for (let k = 0; k < sum; k++) {
				                            let i = msg.substring(k * 2, k * 2 + 2);
				                            arr.push(i);
				                        }
				                        console.log(arr);
				                    }
				                }
				            );
			},
			close(){
				TCPSocket.disconnect({ 
				            channel: '1'//可选 1~20
				});
				if(this.flag==true)
				{
					this.direct='关闭';
				};
			},
			forw(){
				TCPSocket.send({
				                //charsetname:'GBK',//可不选,默认UTF-8
				                channel:'1',//可选 1~20
				                message: 'forw'
				            });
							if(this.flag==true)
							{
								this.direct='前进';
							};
			},
			stop(){
				TCPSocket.send({
				                //charsetname:'GBK',//可不选,默认UTF-8
				                channel:'1',//可选 1~20
				                message: 'stop'
				            });
							if(this.flag==true)
							{
								this.direct='停止';
							};
			},
			back(){
				TCPSocket.send({
				                //charsetname:'GBK',//可不选,默认UTF-8
				                channel:'1',//可选 1~20
				                message: 'back'
				            });
							if(this.flag==true)
							{
								this.direct='后退';
							};
			},
			sleep(){
				TCPSocket.send({
				                //charsetname:'GBK',//可不选,默认UTF-8
				                channel:'1',//可选 1~20
				                message: 'qmcb'
				            });
							if(this.flag==true)
							{
								this.direct='哄睡中';
							};
			},
			left(){
				TCPSocket.send({
				                //charsetname:'GBK',//可不选,默认UTF-8
				                channel:'1',//可选 1~20
				                message: 'left'
				            });
							if(this.flag==true)
							{
								this.direct='左转';
							};
			},
			right(){
				TCPSocket.send({
				                //charsetname:'GBK',//可不选,默认UTF-8
				                channel:'1',//可选 1~20
				                message: 'right'
				            });
							if(this.flag==true)
							{
								this.direct='右转';
							};
			}
		}
	}
</script>

<style>
	button{
		width: 100%;
		height: 100%;
		background:linear-gradient(to bottom right, #b0d383,#f3f0eb);
		font-weight: 600;
		border-radius: 50%;
	}
	button::after{
		border: none;
	}
	.baby{
		width:120px;
		height: 50px;
		position: relative;
		top: 70px;
		float: left;
		margin-left: 15px;
		font-size: 20px;
		font-weight: 600;
	}
	.baby1{
		height:100%;
		width: 50px;
		vertical-align: middle;
	}
	.baby1,.baby2{
		display:inline-block;
	}
	.car{
		width:120px;
		height: 50px;
		position: relative;
		top: 70px;
		float: right;
		margin-right: 10px;
		font-size: 20px;
		font-weight: 600;
	}
	.car1{
		height:100%;
		width: 50px;
		vertical-align: middle;
	}
	.car1,.car2{
		display:inline-block;
	}
	.top{
		position:absolute;
		top:50%;
		left:50%;
		margin-left: -50px;
		margin-top: -140px;
		height: 100px;
		width: 100px;
	}
	.top button{
		background-image: url("../../static/picture1/top.png");
		background-repeat: no-repeat;
		background-size: 100%;
	}
	.last{
		position:absolute;
		top:50%;
		left:50%;
		margin-left: -50px;
		margin-top: 80px;
		height: 100px;
		width: 100px;
	}
	.last button{
		background-image: url("../../static/picture1/bottom.png");
		background-repeat: no-repeat;
		background-size: 100%;
	}
	.stop{
		position:absolute;
		left:50%;
		top:50%;
		margin-left: -50px;
		margin-top:-30px;
		height: 100px;
		width: 100px;
		font-size: 0;
	}
	.stop button{
		background-image: url("../../static/picture1/stop.png");
		background-repeat: no-repeat;
		background-size: 100%;
	}
	.right{
		position:absolute;
		top:50%;
		right: 30px;
		margin-top:-30px;
		height: 100px;
		width: 100px;
	}
	.right button{
		background-image: url("../../static/picture1/right.png");
		background-repeat: no-repeat;
		background-size: 100%;
	}
	.left{
		position:absolute;
		top:50%;
		left: 30px;
		margin-top:-30px;
		height: 100px;
		width: 100px;
	}
	.left button{
		background-image: url("../../static/picture1/left.png");
		background-repeat: no-repeat;
		background-size: 100%;
	}
	.sleep{
		display: inline-block;
		margin: 0 35px 0 35px;
	}
	.sleep:first-child{
		float: left;
	}
	.sleep:last-child{
		float:right;
	}
	.sleep button{
		width: 13vh;
		height: 8vh;
		border-radius:3vh;
		font-size: 17px;
		text-align: center;
		line-height: 8vh;
	}
	.socket{
		display: inline-block;
		margin: 0 25px 0 25px;
	}
	.socket:first-child{
		float: left;
	}
	.socket:last-child{
		float:right;
	}
	.socket button{
		width: 15vh;
		height: 6vh;
		font-size: 17px;
		text-align: center;
		line-height: 6vh;
		background-image: linear-gradient(#688f4d,#b0d383);
		color:#fff;
		border-radius: 2em;
	}
	.sleepy{
		height: 6vh;
		width: 100%;
		position:absolute;
		bottom: 160px;
	}
	.bottom{
		height: 6vh;
		width: 100%;
		position:absolute;
		bottom: 30px;
	}
	.all{
		position: absolute;
		width: 100%;
		height: 100vh;
		background-image: linear-gradient(#effde7,#fbf9ec);
	}
</style>
