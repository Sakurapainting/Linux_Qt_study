#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camera.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QPainter>
#include <QFile>

/* OpenCV 头文件 */
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/* SeetaFace 头文件 */
#include "seeta/FaceDetector.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , faceDetector(nullptr)
    , faceDetectEnabled(false)
{
    ui->setupUi(this);

    layoutInit();
    scanCameraDevice();
    initFaceDetector();
}

MainWindow::~MainWindow()
{
    if (faceDetector) {
        delete faceDetector;
        faceDetector = nullptr;
    }
    delete ui;
}

void MainWindow::layoutInit()
{
    /* 获取屏幕的分辨率 */
    QList <QScreen *> list_screen =  QGuiApplication::screens();

    /* ARM平台 全屏显示 */
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(800, 480);
#endif

    mainWidget = new QWidget();
    rightWidget = new QWidget();
    comboBox = new QComboBox();
    openCameraButton = new QPushButton();
    faceDetectButton = new QPushButton();
    scrollArea = new QScrollArea();
    displayLabel = new QLabel(scrollArea);
    vboxLayout = new QVBoxLayout();
    hboxLayout = new QHBoxLayout();

    vboxLayout->addWidget(comboBox);
    vboxLayout->addWidget(openCameraButton);
    vboxLayout->addWidget(faceDetectButton);
    vboxLayout->addStretch();

    rightWidget->setLayout(vboxLayout);

    hboxLayout->addWidget(scrollArea);
    hboxLayout->addWidget(rightWidget);
    mainWidget->setLayout(hboxLayout);

    this->setCentralWidget(mainWidget);

    openCameraButton->setMaximumHeight(40);
    openCameraButton->setMaximumWidth(200);

    faceDetectButton->setMaximumHeight(40);
    faceDetectButton->setMaximumWidth(200);

    comboBox->setMaximumHeight(40);
    comboBox->setMaximumWidth(200);
    
    scrollArea->setMinimumWidth(this->width() - comboBox->width());

    /* 显示图像最大画面 */
    displayLabel->setMinimumWidth(scrollArea->width() * 0.75);
    displayLabel->setMinimumHeight(scrollArea->height() * 0.75);
    scrollArea->setWidget(displayLabel);

    /* 居中显示 */
    scrollArea->setAlignment(Qt::AlignCenter);

    /* 自动拉伸 */
    displayLabel->setScaledContents(true);

    /* 设置按钮文本 */
    openCameraButton->setText("开启摄像头");
    openCameraButton->setCheckable(true);
    
    faceDetectButton->setText("开启人脸检测");
    faceDetectButton->setCheckable(true);
    faceDetectButton->setEnabled(false);

    /* 摄像头 */
    camera = new Camera(this);

    /* 连接信号和槽 */
    connect(camera, SIGNAL(readyImage(QImage)),
            this, SLOT(receiveImage(QImage)));
    connect(openCameraButton, SIGNAL(clicked()),
            this, SLOT(openCameraButtonClicked()));
    connect(faceDetectButton, SIGNAL(clicked()),
            this, SLOT(faceDetectButtonClicked()));
    connect(comboBox, SIGNAL(currentIndexChanged(int)),
            camera, SLOT(selectCameraDevice(int)));
}

void MainWindow::scanCameraDevice()
{
    /* 扫描摄像头设备，最多支持10个设备 */
    for (int i = 0; i < 10; i++) {
        comboBox->addItem("摄像头设备" + QString::number(i));
    }
}

void MainWindow::initFaceDetector()
{
    /* 初始化人脸检测器 */
    seeta::ModelSetting setting;
    
    /* 设置模型路径 */
    setting.append("/model/fd_2_00.dat");
    setting.set_device(seeta::ModelSetting::CPU);

    /* 创建人脸检测器 */
    faceDetector = new seeta::v2::FaceDetector(setting);
    
    /* 设置检测器参数 */
    faceDetector->set(seeta::v2::FaceDetector::PROPERTY_MIN_FACE_SIZE, 40);
    
    qDebug() << "人脸检测器初始化完成";
}

void MainWindow::receiveImage(const QImage &image)
{
    if (faceDetectEnabled) {
        /* 进行人脸检测并绘制人脸框 */
        QImage processedImage = drawFaceRect(image);
        displayLabel->setPixmap(QPixmap::fromImage(processedImage));
    } else {
        /* 直接显示图像 */
        displayLabel->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::openCameraButtonClicked()
{
    if (openCameraButton->isChecked()) {
        /* 开启摄像头 */
        if (camera->cameraProcess(true)) {
            openCameraButton->setText("关闭摄像头");
            faceDetectButton->setEnabled(true);
        } else {
            openCameraButton->setChecked(false);
            qDebug() << "摄像头打开失败";
        }
    } else {
        /* 关闭摄像头 */
        camera->cameraProcess(false);
        openCameraButton->setText("开启摄像头");
        faceDetectButton->setEnabled(false);
        faceDetectButton->setChecked(false);
        faceDetectEnabled = false;
    }
}

void MainWindow::faceDetectButtonClicked()
{
    if (faceDetectButton->isChecked()) {
        faceDetectEnabled = true;
        faceDetectButton->setText("关闭人脸检测");
    } else {
        faceDetectEnabled = false;
        faceDetectButton->setText("开启人脸检测");
    }
}

QImage MainWindow::drawFaceRect(const QImage &image)
{
    if (!faceDetector) {
        return image;
    }

    /* 创建QImage的副本用于绘制 */
    QImage resultImage = image.copy();

    /* 将QImage转换为SeetaImageData */
    QImage rgbImage = resultImage.convertToFormat(QImage::Format_RGB888);
    
    SeetaImageData seetaImg;
    seetaImg.width = rgbImage.width();
    seetaImg.height = rgbImage.height();
    seetaImg.channels = 3;
    seetaImg.data = rgbImage.bits();

    /* 检测人脸 */
    SeetaFaceInfoArray faces = faceDetector->detect(seetaImg);

    /* 在图像上绘制人脸框 */
    QPainter painter(&resultImage);
    painter.setPen(QPen(Qt::green, 2));

    for (int i = 0; i < faces.size; i++) {
        SeetaFaceInfo &face = faces.data[i];
        painter.drawRect(face.pos.x, face.pos.y, face.pos.width, face.pos.height);
        
        /* 显示人脸置信度 */
        QString scoreText = QString("Score: %1").arg(face.score, 0, 'f', 2);
        painter.setPen(QPen(Qt::yellow, 2));
        painter.drawText(face.pos.x, face.pos.y - 5, scoreText);
        painter.setPen(QPen(Qt::green, 2));
    }

    qDebug() << "检测到" << faces.size << "个人脸";

    return resultImage;
}

