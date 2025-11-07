#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    /* 接收数据接口 */
    void receivedData(int);

    /* 数据最大个数 */
    int maxSize;

    int maxX;
    int maxY;

    /* y轴 */
    QValueAxis *axisY;

    /* x轴 */
    QValueAxis *axisX;

    QList<int> data;
    QSplineSeries *splineSeries;
    QChart *chart;
    QChartView *chartView;
    QTimer *timer;

private slots:
    void timerTimeOut();
};
#endif // MAINWINDOW_H
