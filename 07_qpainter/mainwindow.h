#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    
protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;

    /* 定时器，用于定时更新界面 */
    QTimer *timer;

    int angle;

private slots:
    void timerTimeOut();
};
#endif // MAINWINDOW_H
