## TableView
 - 基于Qt QSqlite驱动实现数据加载
 - 使用了QSqlTableModel或QSqlRelationalTableModel作为实现类
 - 使用Qt Quick 2.0的TableView作为数据呈现组件
 - 默认内置了一个books表，以及使用内存数据库作为默认的数据库(数据表创建参见migrations/001_books.sql)
 
* 截图
 ![demo](https://github.com/yuriyoung/qml-examples/blob/master/assets/img/tableview.jpg)
 
 ## 功能
 - 支持sqlite数据库
 - 支持数据库迁移
 - 支持数据库软删除
 - 支持软删除恢复
 - 支持行选择/删除选行
 - 支持数据库/表切换
 
## TODO
- 添加软删除: 重新实现removeRow接口
- 数据库/表切换时重置model
- 实现一个Migration迁移类
- 实现QML中界面功能实现
  - 编辑更新
  - 增加行
  - 删除行
  - 恢复行
  - 存盘
