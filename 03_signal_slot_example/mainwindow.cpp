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
    /* 信号与槽连接 */
    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
    connect(this, SIGNAL(pushButtonTextChanged()), this, SLOT(changeButtonText()));
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