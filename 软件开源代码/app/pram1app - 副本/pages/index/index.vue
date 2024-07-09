<template>
	<view>
		<!--<hx-navbar ref="hxnb" :config="config">
			<block slot="left">
				<view class="a1">
					<text class="a2">首页</text>
				</view>
			</block>
		</hx-navbar>-->
		<view class="a3">
			<image src="../../static/picture1/banner.png" class="a4"></image>
		</view>
		
			<view class="a5">
				
					<view class="a6" v-for="(item,index) in navarr" :key="index" @click="navClick(item.page)">
				<image :src="item.icon" class="a7"></image>
				<text>{{item.title}}</text>
			</view>
				
			
		</view>
		
		<view class="b1">
			<view class="b2">
				<image src="../../static/lb_bg.png" class="b3"></image>
				<view>关于系统上线运行的通知</view>
			</view>
			<view class="b4">2-23</view>
		</view>

		 <view class="b5">
			<text class="b6">推荐应用</text>
			<text class="b7">根据您最近浏览和办理的应用为您推荐</text>
		</view>
		<view class="b8">
			<view class="b9" v-for="(item,index) in recomarr" :key="index">
				<image :src="item.icon" class="c1"></image>
				<text class="c2">{{item.title}}</text>
				<text class="c3">{{item.value}}</text>
				<text class="c4">{{item.view}}浏览</text>
				<image src="../../static/add.png" class="c5"></image>
			</view>
		</view>
		<view class="b5">
			<text class="b6">新闻资讯</text>
		</view>
		<swiper :current="tab" class="c6">
			<swiper-item>
				<view class="c7" v-for="(item,index) in noticeArr" :key="index">
					<view class="c8">
						<view class="c9">{{item.title}}</view>
						<view class="d1">{{item.author}}</view>
					</view>
					<image :src="item.icon" class="d2"></image>
				</view>
			</swiper-item>
			<swiper-item>
				<view class="c7" v-for="(item,index) in noticeArr" :key="index">
					<view class="c8">
						<view class="c9">{{item.title}}</view>
						<view class="d1">{{item.author}}</view>
					</view>
					<image :src="item.icon" class="d2"></image>
				</view>
			</swiper-item>
			<swiper-item>
				<view class="c7" v-for="(item,index) in noticeArr" :key="index">
					<view class="c8">
						<view class="c9">{{item.title}}</view>
						<view class="d1">{{item.author}}</view>
					</view>
					<image :src="item.icon" class="d2"></image>
				</view>
			</swiper-item>
			<swiper-item>
				<view class="c7" v-for="(item,index) in noticeArr" :key="index">
					<view class="c8">
						<view class="c9">{{item.title}}</view>
						<view class="d1">{{item.author}}</view>
					</view>
					<image :src="item.icon" class="d2"></image>
				</view>
			</swiper-item>
		</swiper> 
	</view>
</template>

<script>
	export default {
		data() {
			return {
				ifplay: true,
				config: {
					back: false,
					fixed: true,
					leftSlot: true,
					search: {
						placeholder: ''
					}
				},
				navarr: [{
						title: '早教课堂',
						icon: '../../static/intex/class.png',
						page:'/pages/profile/profile'
					},
					{
						title: '宝宝健康',
						icon: '../../static/intex/heart.png',
						page:'/pages/score/score'
					},
					{
						title: '育儿笔记',
						icon: '../../static/intex/note.png'
					},
					{
						title: '婴儿圈',
						icon: '../../static/intex/friend.png'
					},
					{
						title: '全部',
						icon: '../../static/intex/all.png'
					}
				],
				recomarr: [{
						title: '早教课堂',
						icon: '../../static/intex/class.png',
						value: '宝宝课堂开课了',
						view: '200'
					},
					{
						title: '宝宝健康',
						icon: '../../static/intex/heart.png',
						value: '阅读育儿小技巧',
						view: '200'
					},
					{
						title: '早教音乐',
						icon: '../../static/intex/music.png',
						value: '热门音乐排行榜',
						view: '200'
					}
				],
				tab: 0,
				noticeArr: [{
						title: '宝宝外出安全大盘点。这些安全隐患你可能根本没注意！',
						author: '人民日报',
						icon: '../../static/intex/zhuyi.png'
					},
					{
						title: '社区开展科学育儿主题宣传活动',
						author: '大洋网',
						icon: '../../static/intex/huodong.png'
					},
					{
						title: '真正自信的孩子，从来不是“夸”出来的',
						author: '大洋网',
						icon: '../../static/intex/warm.png'
					},
					{
						title: '成为“不扫兴的父母”',
						author: '澎湃新闻',
						icon: '../../static/intex/hot.png'
					},
					/*{
						title: '真正自信的孩子，从来不是“夸”出来的',
						author: '大洋网',
						icon: '../../static/intex/xw.jpg'
					}*/
				]
			}
		},
		onLoad() {

		},
		methods: {
			tabChange({
				index,
				item
			}) {
				this.tab = index;
			},
			navClick(page){
				uni.navigateTo({
					url:page
				})
			},
		}
	}
</script>

<style>
	.inner{
		border: 6px solid #7FFF66;
	}
	page {
		background-color: #fffbe4;
	}

	.a1 {
		padding-left: 35upx;
	}

	.a2 {
		font-size: 18px;
		font-weight: 700;
	}

	.a3 {
		display: flex;
		flex-direction: column;
		align-items: center;
	}

	.a4 {
		width: 100%;
		height: 40vh;
	}

	.a5 {
		background-image: url("../../static/picture1/backgroud.png");
		background-repeat: no-repeat;
		background-size: cover;
		display: flex;
		flex-direction: row;
		align-items: center;
		justify-content: space-around;
		width: 98%;
		height: 100px;
		font-size: 22upx;
		background-color: #FFFFFF;
		color: #666666;
		position: absolute;
		margin-top: -20px;
		margin-left: 1px;
		padding-top: 35upx;
		z-index: 2;
		border-radius: 30px;
	}

	.a6 {
		display: flex;
		flex-direction: column;
		align-items: center;
		margin: 15px;
		margin-top: 31px;
	}

	.a7 {
		height: 70upx;
		width: 70upx;
		margin-bottom: 15upx;
	}

	.a8 {
		display: flex;
		flex-direction: row;
		justify-content: space-around;
		margin-top: 40upx;
		padding-left: 20upx;
		padding-right: 20upx;
	}

	.a9 {
		height: 160upx;
		width: 320upx;
		border-radius: 30upx;
	}

	.b1 {
		display: flex;
		background-color: #fff;
		margin: auto;
		width: 90%;
		border-radius: 20px;
		flex-direction: row;
		align-items: center;
		justify-content: space-between;
		margin-top: 105px;
	}

	.b2 {
		display: flex;
		flex-direction: row;
		align-items: center;
		font-weight: 600;
		font-size: 28upx;
		color: #666666;
		
	}

	.b3 {
		height: 50upx;
		width: 50upx;
		margin-right: 20upx;
	}

	.b4 {
		font-size: 24upx;
		color: darkgrey;
	}

	.b5 {
		display: flex;
		flex-direction: column;
		padding-left: 30upx;
		margin-top: 14px;
	}

	.b6 {
		font-size: 23px;
		font-weight: 600;
	}

	.b7 {
		font-size: 20upx;
		color: darkgray;
		margin-top: 10upx;
		letter-spacing: 1upx;
	}

	.b8 {
		display: flex;
		flex-direction: row;
		align-items: center;
		justify-content: space-between;
		padding-top: 70upx;
		padding-left: 30upx;
		padding-right: 30upx;
		margin-bottom: 30upx;
	}

	.b9 {
		display: flex;
		flex-direction: column;
		border-radius: 30upx;
		padding-top: 30upx;
		align-items: center;
		height: 300upx;
		width: 220upx;
		box-shadow: 0px -5px 10px 0px #d4d3d8;
		background-color: #fff;
	}

	.c1 {
		width: 70upx;
		height: 70upx;
	}

	.c2 {
		font-size: 26upx;
		margin-top: 10upx;
	}

	.c3 {
		font-size: 18upx;
		color: darkgray;
	}

	.c4 {
		font-size: 20upx;
		color: #666666;
		margin-top: 20upx;
	}

	.c5 {
		height: 50upx;
		width: 50upx;
		margin-top: 30upx;
		
	}

	.c6 {
		height: 670upx;
		background-color: #fff;
		margin: 15px 20px 20px 20px;
		border-radius: 2em;
	}

	.c7 {
		display: flex;
		flex-direction: row;
		align-items: center;
		padding-left: 35upx;
		padding-right: 35upx;
		margin-top: 45upx;
	}

	.c8 {
		height: 110upx;
		display: flex;
		flex-direction: column;
		width: 76%;
		padding-right: 30upx;
	}

	.c9 {
		font-size: 15px;
		font-weight: 500;
		height: 90upx;
	}

	.d1 {
		font-size: 20upx;
		color: #999;
		height: 20upx;
	}

	.d2 {
		height: 110upx;
		width: 110upx;
		border-radius: 30upx;
	}
	.music-close,
	.music-open {
		width: 45rpx;
		height: 45rpx;
	}
	.music-open {
		animation: minhand 2s linear infinite;
	}
	@-webkit-keyframes minhand {
	 
		0% {
			-webkit-transform: rotate(0deg)
		}
	 
		100% {
			-webkit-transform: rotate(360deg)
		}
	}
	 
	@-moz-keyframes minhand {
		0% {
			-moz-transform: rotate(0deg)
		}
	 
		100% {
			-moz-transform: rotate(360deg)
		}
	}
	 
	@keyframes minhand {
		0% {
			transform: rotate(0deg)
		}
	 
		100% {
			transform: rotate(360deg)
		}
		}
</style>
