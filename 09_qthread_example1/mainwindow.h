#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QThread>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class WorkerThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    WorkerThread *workerThread;
    QPushButton *pushButton;

private slots:
    /* 用于接收线程发送的信号 */
    void handleResults(const QString &result);

    /* 点击按钮开启线程 */
    void pushButtonClicked();
};

class WorkerThread : public QThread
{
    /* 用到信号槽即需要此宏定义 */
    Q_OBJECT

public:
    WorkerThread(QWidget *parent = nullptr) {
        Q_UNUSED(parent);
    }

    /* 重写run方法，继承QThread的类，只有run方法是在新的线程里 */
    void run() override {
        QString result = "线程开启成功";

        /* 这里写上比较耗时的操作 */
        // ...
        // 延时2s，把延时2s当作耗时操作
        sleep(2);

        /* 发送结果准备好的信号 */
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &s);
};

#endif // MAINWINDOW_H
