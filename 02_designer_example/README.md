# Qt Designer

## 连接信号与槽的两种方式

### 在ui文件里

Edit Signals/slots，选中组件向外拖动，进入选择界面

### 转到槽

选中“关闭程序”pushButton 按钮，然后右键

选择“转到槽”。

点击 OK 后，就会跳转到槽函数里，这个代码由 Qt Creator 自动生成。

同时在 mainwindow.h 里声明了这个槽函数。

返回到 mainwindow.cpp 找到 on_pushButton_clicked 这个槽函数里。在这个槽数里写上 this->close();调用 close()方法关闭整个程序。