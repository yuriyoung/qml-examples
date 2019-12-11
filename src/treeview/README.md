## Qt Quick 2.0 TreeView
从Quick controls 2.0的ListView实现，从QAbstractItemModel实现树形结构。截图中的UI使用了Material风格，例子中使用的是默认样式，请自行修改qml UI组件样式。

## 截图
 ![screenhot](https://github.com/yuriyoung/qml-examples/blob/master/assets/img/treeview.gif)
 
## 功能
- 支持json文件
- 支持节点编辑/增加/删除
- 支持本地json文件保存
- 支持本地json文件读取
- 节点展开
- 节点折叠
- 无限节点

## json examples
> 不要忘记在QML中调用时指定json文件(项目中有样例resources/tree.json)或json格式的字符串

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
    - [ ] 增加
    - [ ] 编辑
    - [ ] 删除
- [ ] 节点搜索
- [ ] 多列
- [ ] 水平滚动
