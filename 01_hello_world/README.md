# hello world

## first hello world

Qt -> Qt Widget

默认勾选“Generate form”，意思是生成 ui 窗体文件 mainwindow.ui。为了学习方便，我们统一默认基类为 QMainWindow，但是注意，在嵌入式里一般不需要标题栏，状态栏等，所以常用的是 QWidget 基类。

- QMainWindow：主窗口类，主窗口具有主菜单栏、工具栏和状态栏。类似于一般的应用程序的主窗口。
- QWidget：是可视界面类的基类，也就是说 QMainWindow 类也是由 QWidget 继承封装而来。所以 QWidget 要比 QMainWindow 功能少一些。
- QDialog：对话框类，建立一个对话框界面。比较少使用此项作为基类。一般以 QMainWindow 和 QWidget 作为基类的居多。

暂时先不选翻译文件

### 项目文件*.pro

如果需要修改生成目标的可执行程序名字，可赋值 TARGET = xxx。否则 TARGET 将默认取值为项目的名字。

### 样式文件*.ui

mainwindow.ui 是一个 xml 类型的文件，它的 xml 内容如下。这个文件是生成的不能手动编辑。只能够通过图形界面修改其属性。

### main

```cpp
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
```

- 几乎在使用 Qt 的所有情况下，main()只需要在把控制转交给 Qt 库之前执行一些初始化，然后 Qt 库通过事件来向程序告知用户的行为。
- a 是这个程序的 QApplication。它在这里被创建并且处理这些命令行变量。
- 创建一个对象 w，这个对象就是 MainWindow。
- 调用方法 show()。这样程序界面才能显示。
- 这里就是 main()把控制转交给 Qt，并且当应用程序退出的时候 exec()就会返回。在 exec()中，Qt 接受并处理用户和系统的事件并且把它们传递给适当的窗口部件。

第一次编译时会报“Cannot find -lgL”的错误。
由于 Qt5.0 的库默认会链接到 OpenGL，但是在 Ubuntu 机器上没有安装 OpenGL，所以们需要在 Ubuntu 下安装 OpenGL Library

```bash
sudo apt-get install libglu1-mesa-dev
```
