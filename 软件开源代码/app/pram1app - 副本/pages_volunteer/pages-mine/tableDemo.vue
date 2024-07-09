<template>
	<view class="demo-wrap">
		<single-demo/>
		<custom-cel-width/>
		<loading-table/>
		<empty-row/>
		<fixed-height-empty-row/>
		<fixed-height/>
		<custom-row-style/>
		<custom-row-style-second/>
		<custom-cel/>
		<custom-cel-content/>
		<row-span/>
		<cel-span/>
		 <mulit-checkbox/>
		<single-selected/>
		<edi-data/>
		<single-fixed-right/>
		<single-fixed-left/>
		
	</view>

</template>

<script>
	import vTable from "@/components/table.vue";
	import singleDemo from "@/components/example/single.vue";
	export default {
		components: {
			vTable,
			singleDemo
		}
	}
</script>
<style>
	.demo-wrap{
		padding:20px 10px;
	}
</style>

