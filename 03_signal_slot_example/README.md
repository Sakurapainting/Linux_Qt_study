# 信号与槽

## 在项目中创建信号

由于信号只需声明，无需定义。所以我们只需要在 mainwindow.h 里声明信号即可。

引入 QPushButton 头文件，

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void pushButtonTextChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
```

## 在项目中创建槽

在 mianwindow.h 里直接声明槽，在 mianwindow.cpp 里实现槽的定义，声明槽必须写槽的定义(定义指函数体的实现)，否则编译器编译时将会报错。

槽有以下特点：
1. 槽可以是任何成员函数、普通全局函数、静态函数
2. 槽函数和信号的参数和返回值要一致

声明了信号 void pushButtonTextChanged();

所以我们声明的槽函数必须是无返回值类型 void，和无需参数。

此外我们还声明一个 QPushButton 对象 pushButton。对象 pushButton 可以写成简写 btn。

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void pushButtonTextChanged();

public slots:
    void changeButtonText();

    void pushButtonClicked();

private:
    Ui::MainWindow *ui;
    QPushButton *pushButton;
};
#endif // MAINWINDOW_H
```

```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 设置窗体的宽为 800,高为 480 */
    this->resize(800,480);
    /* 实例化 pushButton 对象 */
    pushButton = new QPushButton(this);
    /* 调用 setText()方法设定按钮的文本 */
    pushButton->setText("我是一个按钮");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushButtonClicked()
{
    /* 发射信号 */
    emit pushButtonTextChanged();
}

void MainWindow::changeButtonText()
{
    /* 修改按钮的文本 */
    pushButton->setText("被点击了");
}
```

## 连接信号与槽

```cpp
QObject::connect(sender, SIGNAL(signal()), receiver, SLOT(slot()));
```

因为 pushButtonClicked()是本类里定义的槽，所以用 this 来接收。同理，pushButtonTextChanged()也是本类定义的信号。所以发送者写成 this。changeButtonText()也是本类的槽函数，所以接收槽的对象也是 this。

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 设置窗体的宽为 800,高为 480 */
    this->resize(800,480);
    /* 实例化 pushButton 对象 */
    pushButton = new QPushButton(this);
    /* 调用 setText()方法设定按钮的文本 */
    pushButton->setText("我是一个按钮");
    /* 信号与槽连接 */
    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
    connect(this, SIGNAL(pushButtonTextChanged()), this, SLOT(changeButtonText()));
}
```

Qt 里有大量的信号与槽，都是 Qt 自定义好的。基本够我们使用，如果没有找到想要的信号与槽，我们可以按上述方法自定义。
