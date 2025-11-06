#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, 800, 480);

    textEdit = new QTextEdit();
    vBoxLayout = new QVBoxLayout();
    hBoxLayout = new QHBoxLayout();
    vWidget = new QWidget();
    hWidget = new QWidget();
    openPushButton = new QPushButton();
    closePushButton = new QPushButton();

    openPushButton->setMinimumHeight(50);
    openPushButton->setMaximumWidth(120);
    closePushButton->setMinimumHeight(50);
    closePushButton->setMaximumWidth(120);

    openPushButton->setText("打开");
    closePushButton->setText("关闭");

    /* 设置关闭按钮为不可用属性，需要打开文件才设置为可用属性 */
    closePushButton->setEnabled(false);

    hBoxLayout->addWidget(openPushButton);
    hBoxLayout->addWidget(closePushButton);
    hWidget->setLayout(hBoxLayout);

    vBoxLayout->addWidget(textEdit);
    vBoxLayout->addWidget(hWidget);
    vWidget->setLayout(vBoxLayout);

    setCentralWidget(vWidget);

    connect(openPushButton, SIGNAL(clicked()),
            this, SLOT(openFile()));
    connect(closePushButton, SIGNAL(clicked()),
            this, SLOT(closeFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    file.setFileName(fileName);

    if (!file.exists())
        return false;

    // 以只读文本模式打开文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // 读取文本到textEdit
    textEdit->setPlainText(file.readAll());

    // 设置打开按钮不可用，需要关闭再打开
    openPushButton->setEnabled(false);

    // 设置关闭按钮为可用属性
    closePushButton->setEnabled(true);

    // 关闭文件
    file.close();

    return true;
}

void MainWindow::closeFile()
{
    // 检测打开按钮是否可用，不可用时，说明已经打开了文件
    if (!openPushButton->isEnabled()) {
        QString str = textEdit->toPlainText();

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        // 转换为字节数组
        QByteArray strBytes = str.toUtf8();

        file.write(strBytes, strBytes.length());

        // 清空textEdit的显示内容
        textEdit->clear();

        file.close();

        // 重新设置打开和关闭按钮的属性
        openPushButton->setEnabled(true);
        closePushButton->setEnabled(false);
    }
}