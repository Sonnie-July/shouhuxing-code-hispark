<template>
	<!--<div>
		/*<view>
			<image v-show="!ifplay" class="music-close" src="../../static/logo.png" @tap="musicState"></image>
			<image v-show="ifplay" class="music-open" src="../../static/logo.png" @tap="musicState"></image>
		</view>
	<div>-->
	<view>
		<view class="page-body">
			<view class="btn-area">
				<button class="btn-logout1" @tap="gonavigate1">
					<image class="pic1" src="../../static/picture1/nega-environment.png"></image>
					<text class="te1">环境状态</text>
					</button>
				<button class="btn-logout2" @tap="gonavigate2">
					<image class="pic" src="../../static/picture1/baby.png"></image>
					<text class="te">宝宝状态</text>
					</button>
					<button class="btn-logout3" @tap="gonavigate3">
						<image class="pic1" src="../../static/picture1/car.png"></image>
						<text class="te1">婴儿车状态</text>
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
				ifplay: true, // 是否开启音乐
			}
		},
		onLoad(options) {
			this.initMusic()
		},
		methods: {
			gonavigate1(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-study/estate'
				});

			},
			gonavigate2(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-study/bstate'
				});
			
			},
			gonavigate3(){
				//在起始页面跳转到test.vue页面并传递参数
				uni.navigateTo({
					url: '/pages_volunteer/pages-study/cstate'
				});
			
			}
		}
	}
</script>

<style>
	.page-body{
		position: absolute;
		background-color: #f1f1f1;
		width: 100%;
		height: 100vh;
	}
	.te1{
		font-size: 22px;
		float: right;
		height: 100%;
		text-align: center;
		margin: auto 35px auto 0;
		writing-mode: vertical-lr;
		background-color: #f1debe;
		color:#344b4f;
		font-weight: 600;
	}
	.pic1{
		position: absolute;
		width: 75px;
		height: 75px;
		top:20px;
		margin-left: 2vh;
		background-color: #f1debe;
		border-radius: 50%;
		padding:20px;
	}
	.pic{
		position: absolute;
		width: 75px;
		height: 75px;
		right:2vh;
		top:20px;
		margin-right: 2vh;
		background-color: #f1debe;
		border-radius: 50%;
		padding:20px;
	}
	.te{
		font-size: 22px;
		float: left;
		height: 100%;
		text-align: center;
		margin: auto 0 auto 35px;
		writing-mode: vertical-lr;
		color:#344b4f;
		background-color: #f1debe;
		font-weight: 600;
	}
	
.uni-picker-tips {
	font-size: 12px;
	color: #666;
	margin-bottom: 15px;
	padding: 0 15px;
	/* text-align: right; */
}
.btn-logout1 {
	margin-top: 15px;
	width: 90%;
	height: 150px;
	//border: outset;
	color: #fff;
	background:linear-gradient(to bottom right, #afe5c2,#f1ccb8 );
	text-align: left;
	border-radius: 2em;
	box-shadow: 3px 10px 15px 1px rgb(0,0,0,0.1);
	}
.btn-logout2 {
	margin-top: 20px;
	width: 90%;
	height: 150px;
	//border: outset;
	color: #fff;
	background:linear-gradient(to bottom left, #afe5c2,#f1ccb8);
	text-align: left;
	border-radius: 2em;
	box-shadow: 3px 10px 15px 1px rgb(0,0,0,0.1);
}
.btn-logout3 {
	margin-top: 20px;
	width: 90%;
	height: 150px;
	//border: outset;
	color: #fff;
	background:linear-gradient(to bottom right, #afe5c2,#f1ccb8);
	text-align: left;
	border-radius: 2em;
	box-shadow: 3px 10px 15px 1px rgb(0,0,0,0.1);
	}
.btn-logout4 {
	margin-top: 80upx;
	width: 90%;
	height: 100px;
	border: dashed;
	border-width: 2px 4px;
	border-radius: 40px;
	color: #fff;
	background:#ffb7dd;
	text-align: left;
	line-height: 100px;
	}
.btn-logout5 {
	margin-top: 80upx;
	width: 90%;
	height: 100px;
	border: dashed;
	border-width: 2px 4px;
	border-radius: 40px;
	color: #fff;
	background:#ffb7dd;
	text-align: left;
	line-height: 100px;
	}
	.a4 {
		height: 150px;
		width: 380px;
		border-radius: 30px;
	}
</style>
