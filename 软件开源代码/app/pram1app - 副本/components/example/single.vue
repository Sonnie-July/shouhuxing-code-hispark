<template>
	<view class="example">
		<view class="title">历史数据</view>
		<v-table :columns="columns" :list="data"></v-table>
	</view>
</template>

<script>
	import vTable from "@/components/table.vue"
	export default{
		components: {
			vTable
		},
		data(){
			return {
				data: [{
						T: '26.3',
						W: 200,
						S: '230',
						id: "5月1日",
						
					},
					{
						T: '26.3',
						W: 200,
						S: '230',
						id: "5月2日",
					},
					{
						T: '26.3',
						W: 200,
						S: '230',
						id: "5月3日",
					},
					{
						T: '26.3',
						W: 200,
						S: '230',
						id: "5月4日",
					},
					{
						T: '26.3',
						W: 200,
						S: '230',
						id: "5月5日",
					},
				
					{
						T: '26.3',
						W: 200,
						S: '230',
						id: "5月6日",
					}
				],
				columns: [{
						title: "日期",
						key: "id"
					},
					{
						title: '温度',
						key: 'T'
					},
					{
						title: '湿度',
						key: 'W'
					},
					{
						title: '光照强度',
						key: 'S'
					}
				],
			}
		}
		
	}
</script>

<style>
</style>
