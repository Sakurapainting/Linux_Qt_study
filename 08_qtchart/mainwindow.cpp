#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, 800, 480);

    maxSize = 51;
    maxX = 5000;
    maxY = 40;

    splineSeries = new QSplineSeries();

    chart = new QChart();
    chartView = new QChartView();

    axisY = new QValueAxis();
    axisX = new QValueAxis();

    timer = new QTimer(this);

    /* legend译图例类型，以绘图的颜色区分，本例设置为隐藏 */
    chart->legend()->hide();
    chart->setTitle("实时动态曲线示例");
    chart->addSeries(splineSeries);

    axisY->setLabelFormat("%i");
    axisY->setTitleText("温度/℃");

    chart->addAxis(axisY, Qt::AlignLeft);

    axisY->setRange(0, maxY);

    /* 将splineSeries附加于y轴上 */
    splineSeries->attachAxis(axisY);

    axisX->setLabelFormat("%i");

    axisX->setTitleText("时间/ms");

    chart->addAxis(axisX, Qt::AlignBottom);

    axisX->setRange(0, maxX);
    
    /* 将splineSeries附加于x轴上 */
    splineSeries->attachAxis(axisX);

    /* 将图表的内容设置在图表视图上 */
    chartView->setChart(chart);
    /* 设置抗锯齿 */
    chartView->setRenderHint(QPainter::Antialiasing);

    /* 设置为图表视图为中心部件 */
    setCentralWidget(chartView);

    timer->start(200);

    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));

    /* 设置随机种子，随机数初始化 */
    qsrand(time(NULL));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerTimeOut()
{
    /* 产生随机0~maxY之间的数据 */
    receivedData(qrand() % maxY );
}

void MainWindow::receivedData(int value)
{
    /* 将数据添加到data中 */
    data.append(value);

    /* 当储存数据的个数大于最大值时，把第一个数据删除 */
    while (data.size() > maxSize) {
        /* 移除data中第一个数据 */
        data.removeFirst();
    }

    /* 先清空 */
    splineSeries->clear();

    /* 计算x轴上的点与点之间显示的间距 */
    int xSpace = maxX / (maxSize - 1);

    /* 添加点，xSpace * i 表示第i个点的x轴的位置 */
    for (int i = 0; i < data.size(); ++i) {
        splineSeries->append(xSpace * i, data.at(i));
    }
}