# 继承QThread的线程

只有 run()方法在线程里的。其他类内定义的方法都在主线程内。

一般是把耗时的操作写在这个 run()方法里面。

qthread_example1, 本例通过 QThread 类继承线程，然后在 MainWindow 类里使用。通过点击一个按钮开启线程。当线程执行完成时，会发送 resultReady(const QString &s)信号给主线程。

## 信号与槽连接的新语法

使用新式语法,它更安全、更现代。旧式语法仍然可用,但主要是为了兼容老代码。

```cpp
    // connect(workerThread, SIGNAL(resultReady(QString)),
    //         this, SLOT(handleResults(QString)));
    // connect(pushButton, SIGNAL(clicked()),
    //         this, SLOT(pushButtonClicked()));

    // 新式语法 - 使用函数指针
    connect(workerThread, &WorkerThread::resultReady,
            this, &MainWindow::handleResults);
    connect(pushButton, &QPushButton::clicked,
            this, &MainWindow::pushButtonClicked);
```

## 程序运行效果

点击开启线程按钮后，延时 2s 后，Qt Creator 的应用程序输出窗口打印出“线程开启成功”。在 2s 内多次点击按钮则不会重复开启线程，因为线程在这 2s 内还在运行。同时我们可以看到点击按钮没卡顿现象。因为这个延时操作是在我们创建的线程里运行的，而 pushButton 是在主
线程里的，通过点击按钮控制子线程的运行。

当关闭程序后，子线程将在主线程的析构函数里退出。注意线程使用 wait()方法，这里等待 2s，因为我们开启的线和是延时 2s 就完成了。如果是实际的操作，请根据 CPU 的处理能力，给一个适合的延时，阻塞等待线程完成后，就会自动退出并打印“线程已经结束”。
