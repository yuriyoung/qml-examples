## Qt Quick 2.0 TreeView
从Quick controls 2.0的ListView实现，从QAbstractItemModel实现树形结构。

## 截图
 ![screenhot](https://github.com/yuriyoung/qml-examples/blob/master/assets/img/treeview.gif)
 
## 功能
- 支持json文件
- 支持节点编辑/增加/删除
- 支持本地json文件保存
- 支持本地json文件读取
- 节点展开
- 节点收起
- 无限节点

## json examples
> 子节点key不一定必须是"children"，可以是任意key名称。
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

- json tree file:
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
- [ ] UI实现
- [ ] 节点搜索