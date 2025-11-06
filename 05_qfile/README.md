# QFile

QFile 类提供了读取和写入文件的接口。在嵌入式里如果需要读写文件，最简单的方法就是用 Qfile

QFile 来读写 Linux 下的字符设备（可把字符设备当作一个文本处理，linux 下一切皆文件），虽然只是写‘0’或‘1’，但也是对文件（文本）的读写了。

QFile 是一个读写文本、二进制文件和资源的 I/O 设备。QFile 可以自己使用，也可以更方便地与 QTextStream 或 QDataStream 一起使用。

不管操作系统是什么，QFile的文件分隔符都是'/'。

```cpp
private:
    /* 用于读取文件后显示 */
    QTextEdit *textEdit;

    /* QFile类型对象 */
    QFile file;

    /* 水平布局 */
    QHBoxLayout *hBoxLayout;

    /* 垂直布局 */
    QVBoxLayout *vBoxLayout;

    /* 水平布局Widget */
    QWidget *hWidget;

    /* 垂直布局Widget */
    QWidget *vWidget;

    /* 打开文件按钮 */
    QPushButton *openPushButton;

    /* 关闭文件按钮 */
    QPushButton *closePushButton;

private slots:

    /* 打开文本文件 */
    bool openFile();

    /* 关闭文本文件 */
    void closeFile();
```

当使用布局管理器（如 QVBoxLayout、QHBoxLayout）时，控件会自动调整大小。设置最大/最小尺寸可以防止控件被过度拉伸或压缩。

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置窗口的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 布局设置 */
    textEdit = new QTextEdit();
    vBoxLayout = new QVBoxLayout();
    hBoxLayout = new QHBoxLayout();
    vWidget = new QWidget();
    hWidget = new QWidget();
    openPushButton = new QPushButton();
    closePushButton = new QPushButton();

    /* 设置两个按钮的大小 */
    openPushButton->setMinimumHeight(50);
    openPushButton->setMaximumWidth(120);
    closePushButton->setMinimumHeight(50);
    closePushButton->setMaximumWidth(120);

    /* 设置两个按钮的文本 */
    openPushButton->setText("打开");
    closePushButton->setText("关闭");

    /* 设置关闭按钮为不可用属性，需要打开文件才设置为可用属性 */
    closePushButton->setEnabled(false);

    /* 水平布局 */
    hBoxLayout->addWidget(openPushButton);
    hBoxLayout->addWidget(closePushButton);
    hWidget->setLayout(hBoxLayout);

    /* 垂直布局 */
    vBoxLayout->addWidget(textEdit);
    vBoxLayout->addWidget(hWidget);
    vWidget->setLayout(vBoxLayout);

    /* 居中 */
    setCentralWidget(vWidget);

    /* 信号槽连接 */
    connect(openPushButton, SIGNAL(clicked()),
            this, SLOT(openFile()));
    connect(closePushButton, SIGNAL(clicked()),
            this, SLOT(closeFile()));
}
```

```cpp
bool MainWindow::openFile()
{
    /* 获取文件的路径 */
    QString fileName = QFileDialog::getOpenFileName(this);

    /* 指向文件 */
    file.setFileName(fileName);

    /* 判断文件是否存在 */
    if (!file.exists())
        return false;

    /* 以读写的方式打开 */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    /* 读取文本到textEdit */
    textEdit->setPlainText(file.readAll());

    /* 设置打开按钮不可用，需要关闭再打开 */
    openPushButton->setEnabled(false);

    /* 设置关闭按钮为可用属性 */
    closePushButton->setEnabled(true);

    /* 关闭文件 */
    file.close();

    return true;
}

void MainWindow::closeFile()
{
    /* 检测打开按钮是否可用，不可用时，说明已经打开了文件 */
    if (!openPushButton->isEnabled()) {
        /* 获取textEdit的文本内容 */
        QString str = textEdit->toPlainText();

        /* 以只读的方式打开 */
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        /* 转换为字节数组 */
        QByteArray strBytes = str.toUtf8();

        /* 写入文件 */
        file.write(strBytes, strBytes.length());

        /* 清空textEdit的显示内容 */
        textEdit->clear();

        /* 关闭文件 */
        file.close();

        /* 重新设置打开和关闭按钮的属性 */
        openPushButton->setEnabled(true);
        closePushButton->setEnabled(false);
    }
}
```

但是在实际项目里不是用 QPushButton来做打开文件和关闭文件的，一般设计于在菜单栏里用 QAction 来做。包括添加复制、粘贴、另存为、关闭、等等。可以仿照 Windows 里的记事本，用 Qt 写一个类似的软件完全可以。