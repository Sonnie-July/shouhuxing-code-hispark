<template>
	<view class="content">
			<div>
				<view class="d1" style="text-align: center; font-size: 30px;"
>
					婴儿车注意事项
				</view>
				<view class="a9">
					
				
				</view>
				<image src="../../static/picture1/banner.png"></image>	
				<view class="a9">
					
				
				</view>
			</div>	
				<div>
				<view class="row-parent-1 row-item">
					<view class="box1">
						<text style="font-size:20px;">
							婴儿车安装部件为前轮，后轮，前扶手，车体，蓬头、睡篮，OLED屏, Pegasus主板，Taurus摄像头，人体红外，雨滴传感器， DHT11温湿度传感器，GY302-BH1750光照强度传感器。
							
							  先将婴儿车的车架上下关节安装完成，再安装前、后轮，安装车蓬头，整体安装完成后，Pegasus主板位于婴儿车后部底座，用于整个系统控制，并将数据传送到阿里云；
							OLED屏位于婴儿车推手右侧，供父母查看婴儿及婴儿车状况信息；DHT11温湿度传感器位于婴儿床中部，用于检测婴儿体温；
							雨滴传感器位于婴儿床后部，用于检测婴儿是否尿床；人体红外位于侧边栏，Taurus摄像头位于人体红外左侧，用于检测是否有陌生人靠近；
							GY302-BH1750光照强度传感器位于水平婴儿眼部位置的婴儿床侧部，用于检测婴儿车内亮度。
							同时可以直接通过web注册并登录，进行婴儿车实时情况和相关数据的查看。
						</text>
					</view>
					</view>
					</div>
	</view>
</template>

<script>
</script>

<style>
	.content image{
		width: 100%;
		height: 200px;
	}
	.button-high{
	  height: 80px;
	}
	.content {
			display: flex;
			flex-direction: column;
			align-items: center;
			justify-content: center;
			background-color: #ffffff;
		} 
 		.text-area {
			display: flex;
			justify-content: center;
		}
			.row-parent-1 {
			        width: 750rpx;
			        height: fit-content;
					background-color: beige;
			    }
		.row-item {
		        display: flex;
		        flex-direction: row;
			}
		.box1{
			display: flex;
			justify-content: center;
			width: 90%;
			height: 690px;
			background:linear-gradient(to bottom right, #afe5c2,#f1ccb8 );
			border-radius:10px 10px 10px 10px ;
			position: absolute; 
			left: 15px;
		}
		.a9{
			border-bottom: 1rem solid #eff0f1;
			width:400px;
		}
</style>