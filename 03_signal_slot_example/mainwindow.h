#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void pushButtonTextChanged();

public slots:
    void changeButtonText();

    void pushButtonClicked();

private:
    Ui::MainWindow *ui;
    QPushButton *pushButton;
};
#endif // MAINWINDOW_H
