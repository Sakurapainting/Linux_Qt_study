#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, 800, 480);

    pushButton = new QPushButton(this);
    workerThread = new WorkerThread(this);

    pushButton->resize(100, 40);
    pushButton->setText("开启线程");

    // connect(workerThread, SIGNAL(resultReady(QString)),
    //         this, SLOT(handleResults(QString)));
    // connect(pushButton, SIGNAL(clicked()),
    //         this, SLOT(pushButtonClicked()));

    // 新式语法 - 使用函数指针
    connect(workerThread, &WorkerThread::resultReady,
            this, &MainWindow::handleResults);
    connect(pushButton, &QPushButton::clicked,
            this, &MainWindow::pushButtonClicked);
}

MainWindow::~MainWindow()
{
    /* 进程退出，注意本例run()方法没写循环，此方法需要有循环才生效 */
    workerThread->quit();

    /* 阻塞等待2000ms检查一次线程是否已经退出 */
    if (workerThread->wait(2000)) {
        qDebug()<<"线程已经结束！"<<endl;
    }

    delete ui;
}

void MainWindow::handleResults(const QString &result)
{
    /* 打印出线程发送过来的结果 */
    qDebug()<<result<<endl;
}

void MainWindow::pushButtonClicked()
{
    /* 检查线程是否在运行，如果没有则开始运行 */
    if (!workerThread->isRunning())
        workerThread->start();
}