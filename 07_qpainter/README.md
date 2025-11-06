# QPainter

一般用于显示的类，如 QWidget、QPixmap、QImage、Qlabel 等可视类控件都可以充当绘图区域的“画布”，从 QWidget 继承的类都有 virtual void paintEvent(QPaintEvent *event);属性。这个 paintEvent()是一个虚函数，它在 qwidget.h 头文件的 protected:修饰符下。

paintEvent()事件可以被重写。（解释：什么是绘图事件？可以这么理解，当界面初始化或者需要刷新时才会执行的事件，也就是说绘图事件在构造对象实例化时会执行，需要刷新界面我们可以使用 update()方法执行 paintEvent()事件）。

paintEvent()事件是父类 QWidget 提供给子类的接口，在父类里定义为空，所以可以说paintEvent()事件就是专门给子类画图用的。

paintEvent()事件在子类重写的基本结构如下：

```cpp
void Widget::paintEvent(QPaintEvent *)
{
    /* 指定画图的对象，this 代表是本 Widget */
    QPainter painter(this);
    // 使用 painter 在对象上绘图...
}
```

本例使用一张CD 图片，用 QPainter 在 paintEvent()将 CD 画在窗口的中心，并且每 100ms 旋转 1 度角度。所以 CD 看起来是旋转了的效果。

引入头文件

```cpp
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
```

```cpp
/* 重写父类下的protected方法*/
protected:
    void paintEvent(QPaintEvent *);

private:
    /* 定时器，用于定时更新界面 */
    QTimer *timer;
    /* 角度 */
    int angle;

private slots:
    /* 槽函数 */
    void timerTimeOut();
```

定时 100ms 更新一次界面。因为 paintEvent 事件在构造函数执行时只会执行一次。我们需要使用 update()方法来更新界面，才能看到 CD 旋转的效果。

## 添加资源文件

右键项目，选择 `Add New...`

选择一个模板，选择 Qt 模板，再选择 Qt Resource Files

新建完成了资源文件后，默认会进入 res.qrc 文件编辑模式（如果关闭了，可以右键这个文件点击选择“Open in Editor”），点击 Add Prefix 添加前缀，添加前缀的目的是方便分类管理文件

添加了前缀后，添加资源图片，放在/images 前缀的下面

然后点 Add Files 把资源图片添加进来