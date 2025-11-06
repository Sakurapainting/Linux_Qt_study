#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 设置宽高为800×480,位置在0, 0。（0, 0）代表原点，Qt默认最左上角的点为原点 */
    this->setGeometry(0, 0, 800, 480);

    pushButton1 = new QPushButton("窗口皮肤1", this);
    pushButton2 = new QPushButton("窗口皮肤2", this);

    pushButton1->setGeometry(300,200,80,40);
    pushButton2->setGeometry(400,200,80,40);

    connect(pushButton1, SIGNAL(clicked()), this, SLOT(pushButton1_Clicked()));
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(pushButton2_Clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushButton1_Clicked()
{
    /* 设置主窗口的样式1 */
    this->setStyleSheet("QMainWindow { background-color: rgba(255, 245, 238, 100%); }");
}

void MainWindow::pushButton2_Clicked()
{
    /* 设置主窗口的样式2 */
    this->setStyleSheet("QMainWindow { background-color: rgba(238, 122, 233, 100%); }");
}