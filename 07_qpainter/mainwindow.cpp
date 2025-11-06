#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, 800, 480);
    setPalette(QPalette(Qt::gray));
    setAutoFillBackground(true);

    timer = new QTimer(this);

    /* 默认角度为0 */
    angle = 0;

    /* 定时100ms */
    timer->start(100);

    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerTimeOut()
{
    this->update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    /* 设置抗锯齿，流畅转换 */
    painter.setRenderHints(QPainter::Antialiasing
                           | QPainter::SmoothPixmapTransform);
    /* 计算旋转角度 */
    if (angle >= 360)
        angle = 0;

    angle++;

    QPixmap image;

    image.load(":/image/cd.png");

    /* QRectF即，继承QRect（Qt的矩形类），F代表精确到浮点类型 */
    QRectF rect((this->width() - image.width()) / 2,
                (this->height() - image.height()) / 2,
                image.width(),
                image.height());

    /* 默认参考点为左上角原点（0,0），因为旋转需要以图形的中心为参考点，
     * 我们使用translate把参考点设置为CD图形的中心点坐标 */
    painter.translate(0 + rect.x() + rect.width() / 2,
                      0 + rect.y() + rect.height() / 2);

    /* 旋转角度 */
    painter.rotate(angle);

    /* 现在参考点为CD图形的中心，我们需要把它设置回原点的位置，
     * 所以需要减去上面加上的数 */
    painter.translate(0 - (rect.x() + rect.width() / 2),
                      0 - (rect.y() + rect.height() / 2));

    /* 画图,QPainter提供了许多drawX的方法 */
    painter.drawImage(rect, image.toImage(), image.rect());

    /* 再画一个矩形 */
    painter.drawRect(rect.toRect());
}

