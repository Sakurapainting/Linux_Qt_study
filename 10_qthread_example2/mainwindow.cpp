#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, 800, 480);
    pushButton1 =  new QPushButton(this);
    pushButton2 =  new QPushButton(this);

    pushButton1->setGeometry(300, 200, 80, 40);
    pushButton2->setGeometry(400, 200, 80, 40);

    pushButton1->setText("开启线程");
    pushButton2->setText("打断线程");

    worker = new Worker;

    /* 将worker类移至线程workerThread */
    worker->moveToThread(&workerThread);

    /* 线程完成销毁对象 */
    connect(&workerThread, SIGNAL(finished()),
            worker, SLOT(deleteLater()));
    connect(&workerThread, SIGNAL(finished()),
            &workerThread, SLOT(deleteLater()));

    /* 发送开始工作的信号，开始工作 */
    connect(this, SIGNAL(startWork(QString)),
            worker, SLOT(doWork1(QString)));

    /* 接收到worker发送过来的信号 */
    connect(worker, SIGNAL(resultReady(QString)),
            this, SLOT(handleResults(QString)));

    /* 点击按钮开始线程 */
    connect(pushButton1, SIGNAL(clicked()),
            this, SLOT(pushButton1Clicked()));

    /* 点击按钮打断线程 */
    connect(pushButton2, SIGNAL(clicked()),
            this, SLOT(pushButton2Clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;

    /* 打断线程再退出 */
    worker->stopWork();
    workerThread.quit();

    /* 阻塞线程2000ms，判断线程是否结束 */
    if (workerThread.wait(2000)) {
        qDebug()<<"线程结束"<<endl;
    }
}

void MainWindow::pushButton1Clicked()
{
    const QString str = "正在运行";

    /* 判断线程是否在运行 */
    if(!workerThread.isRunning()) {
        /* 开启线程 */
        workerThread.start();
    }

    /* 发送正在运行的信号，线程收到信号后执行后返回线程耗时函数 + 此字符串 */
    emit this->startWork(str);
}

void MainWindow::pushButton2Clicked()
{
    /* 如果线程在运行 */
    if(workerThread.isRunning()) {

        /* 停止耗时工作，跳出耗时工作的循环 */
        worker->stopWork();
    }
}

void MainWindow::handleResults(const QString & results)
{
    /* 打印线程的状态 */
    qDebug()<<"线程的状态："<<results<<endl;
}

