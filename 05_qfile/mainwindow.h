#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>
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

private:
    Ui::MainWindow *ui;

    QTextEdit *textEdit;
    QFile file;

    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;
    QWidget *hWidget;
    QWidget *vWidget;

    QPushButton *openPushButton;
    QPushButton *closePushButton;

private slots:
    bool openFile();
    void closeFile();
};
#endif // MAINWINDOW_H
