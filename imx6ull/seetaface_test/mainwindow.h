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
#include <QLineEdit>
#include <QMap>
#include <QVector>

/* SeetaFace 头文件 */
namespace seeta {
    namespace v2 {
        class FaceDetector;
        class FaceLandmarker;
        class FaceRecognizer;
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

    /* 摄像头设备变更槽函数 */
    void onCameraDeviceChanged(const QString &deviceName);

    /* 录入人脸按钮槽函数 */
    void registerFaceButtonClicked();

    /* 人脸识别开关按钮槽函数 */
    void faceRecognizeButtonClicked();

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
    QPushButton *registerFaceButton;
    QPushButton *faceRecognizeButton;
    QLineEdit *nameLineEdit;
    QLabel *infoLabel;

    /* 摄像头类 */
    Camera *camera;

    /* SeetaFace 人脸检测器 */
    seeta::v2::FaceDetector *faceDetector;

    /* SeetaFace 人脸特征点检测器 */
    seeta::v2::FaceLandmarker *faceLandmarker;

    /* SeetaFace 人脸识别器 */
    seeta::v2::FaceRecognizer *faceRecognizer;

    /* 人脸检测开关 */
    bool faceDetectEnabled;

    /* 人脸识别开关 */
    bool faceRecognizeEnabled;

    /* 人脸特征数据库: 姓名 -> 特征向量 */
    QMap<QString, QVector<float>> faceDatabase;

    /* 当前帧图像（用于录入） */
    QImage currentFrame;

    /* 初始化布局 */
    void layoutInit();

    /* 初始化摄像头设备 */
    void scanCameraDevice();

    /* 初始化人脸检测器 */
    void initFaceDetector();

    /* 在图像上绘制人脸框 */
    QImage drawFaceRect(const QImage &image);

    /* 提取人脸特征 */
    bool extractFaceFeature(const QImage &image, QVector<float> &feature);

    /* 人脸识别 */
    QString recognizeFace(const QVector<float> &feature);

    /* 保存人脸数据库 */
    void saveFaceDatabase();

    /* 加载人脸数据库 */
    void loadFaceDatabase();
};
#endif // MAINWINDOW_H
