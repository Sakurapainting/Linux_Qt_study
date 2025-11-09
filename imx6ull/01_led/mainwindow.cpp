#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList <QScreen *> list_screen =  QGuiApplication::screens();
    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
    #if __arm__
        /* 重设大小 */
        this->resize(list_screen.at(0)->geometry().width(),
                    list_screen.at(0)->geometry().height());
        /* 默认是出厂系统的LED心跳的触发方式,想要控制LED，
        * 需要改变LED的触发方式，改为none，即无 */
        system("echo none > /sys/class/leds/red/trigger");
    #else
        /* 否则则设置主窗体大小为800x480 */
        this->resize(800, 480);
    #endif

    pushButton = new QPushButton(this);

    /* 居中显示 */
    pushButton->setMinimumSize(200, 50);
    pushButton->setGeometry((this->width() - pushButton->width()) /2 ,
                            (this->height() - pushButton->height()) /2,
                            pushButton->width(),
                            pushButton->height()
                            );
    /* 开发板的LED控制接口 */
    file.setFileName("/sys/devices/platform/dtsleds/leds/red/brightness");

    if (!file.exists())
        /* 设置按钮的初始化文本 */
        pushButton->setText("未获取到LED设备！");

    getLedState();

    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLedState()
{
    /* 在设置LED状态时先读取 */
    bool state = getLedState();

    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QByteArray buf[2] = {"0", "1"};

    /* 写0或1 */
    if (state)
        file.write(buf[0]);
    else
        file.write(buf[1]);

    /* 关闭文件 */
    file.close();

    /*重新获取LED的状态 */
    getLedState();
}

bool MainWindow::getLedState()
{
    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return false;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QTextStream in(&file);

    /* 读取文件所有数据 */
    QString buf = in.readLine();

    /* 打印出读出的值 */
    qDebug()<<"buf: "<<buf<<endl;
    file.close();
    if (buf == "1") {
        pushButton->setText("LED点亮");
        return true;
    } else {
        pushButton->setText("LED熄灭");
        return false;
    }
}

void MainWindow::pushButtonClicked()
{
    /* 设置LED的状态 */
    setLedState();
}
