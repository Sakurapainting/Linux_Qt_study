#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QImage>

/* SeetaFace 头文件 */
namespace seeta {
    namespace v2 {
        class FaceDetector;
    }
}

namespace cv {
    class Mat;
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Camera;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /* 接收图像信号的槽函数 */
    void receiveImage(const QImage &image);

    /* 摄像头开启按钮槽函数 */
    void openCameraButtonClicked();

    /* 人脸检测开关按钮槽函数 */
    void faceDetectButtonClicked();

private:
    Ui::MainWindow *ui;

    /* UI 组件 */
    QWidget *mainWidget;
    QScrollArea *scrollArea;
    QLabel *displayLabel;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QWidget *rightWidget;
    QComboBox *comboBox;
    QPushButton *openCameraButton;
    QPushButton *faceDetectButton;

    /* 摄像头类 */
    Camera *camera;

    /* SeetaFace 人脸检测器 */
    seeta::v2::FaceDetector *faceDetector;

    /* 人脸检测开关 */
    bool faceDetectEnabled;

    /* 初始化布局 */
    void layoutInit();

    /* 初始化摄像头设备 */
    void scanCameraDevice();

    /* 初始化人脸检测器 */
    void initFaceDetector();

    /* 在图像上绘制人脸框 */
    QImage drawFaceRect(const QImage &image);
};
#endif // MAINWINDOW_H
