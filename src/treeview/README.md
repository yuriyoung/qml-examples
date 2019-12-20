## Qt Quick 2.0 TreeView
从Quick controls 2.0的ListView实现，从QAbstractItemModel实现树形结构。例子使用的是json tree的结构作为数据呈现。排序/搜索过滤使用QSortFilterProxyModel实现。功能正在完善中...

## 截图
> expand/collapse/edit/add/remove/search

 ![screenhot](https://github.com/yuriyoung/qml-examples/blob/master/assets/img/treeview_1.gif)
 
## 功能
- 支持json文件: TreeView的代理delegate中通过```model.<role>```方式访问json的key读写json的值。eg: 读```model.title```; 写```model.title="new title"```
- 支持节点编辑/增加/删除
- 支持本地json文件保存
- 支持本地json文件读取
- 节点展开
- 节点折叠
- 支持展开折叠状态保存
- 无限节点

## json examples
> 不要忘记在QML中调用时指定json文件(项目中有样例```resources/tree.json```)或json格式的字符串

- tree node:
	```json
	{
		"title": "parent 1",
		"subtitle": "subtitle 1",
		"expanded": false,
		"custom": "value",
		"children": [
			{
				"title": "parent 1",
				"subtitle": "subtitle 1",
				"expanded": false,
				"custom": "value",
				"children": [
					{ },
					{ },
				]
			},
			{  },
		]
	}
	```

- json tree:
	```json
	[
		{
			"title": "parent 1",
			"subtitle": "subtitle 1",
			"expanded": false,
			"custom": "value",
			"children": [
				{ },
				{ },
			]
		},
		{
			"title": "parent 2",
			"subtitle": "subtitle 2",
			"expanded": false,
			"custom": "value",
			"children": [
				{ },
				{ },
			]
		},
		{
			"title": "parent 3",
			"subtitle": "subtitle 3",
			"expanded": false,
			"custom": "value",
			"children": [
				{ },
				{ },
			]
		},
	]
	```
## TODO
- [x] UI实现
	- [x] 显示数据
    - [x] 增加
    - [x] 编辑
    - [x] 删除
- [x] 节点搜索
- [ ] 排序
- [ ] 多列
- [ ] 水平滚动
