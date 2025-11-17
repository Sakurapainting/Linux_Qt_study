#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QLabel>

class VirtualKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualKeyboard(QWidget *parent = nullptr);
    ~VirtualKeyboard();

    /* 设置关联的输入框 */
    void setLineEdit(QLineEdit *lineEdit);

    /* 显示键盘 */
    void showKeyboard();

    /* 隐藏键盘 */
    void hideKeyboard();

signals:
    void keyboardClosed();

private slots:
    /* 按键点击处理 */
    void onKeyClicked();

    /* 删除按钮 */
    void onBackspaceClicked();

    /* 清空按钮 */
    void onClearClicked();

    /* 完成按钮 */
    void onDoneClicked();

    /* 切换键盘模式 */
    void onSwitchModeClicked();

private:
    QLineEdit *targetLineEdit;
    QVBoxLayout *mainLayout;
    QGridLayout *keyLayout;
    QHBoxLayout *controlLayout;
    
    QPushButton *backspaceButton;
    QPushButton *clearButton;
    QPushButton *doneButton;
    QPushButton *switchModeButton;
    
    QLabel *titleLabel;

    /* 键盘模式：0-数字，1-小写字母，2-大写字母 */
    int currentMode;

    /* 创建按键 */
    void createKeys();

    /* 创建数字键盘 */
    void createNumericKeys();

    /* 创建字母键盘 */
    void createAlphaKeys(bool uppercase);

    /* 清空键盘布局 */
    void clearKeyLayout();
};

#endif // VIRTUALKEYBOARD_H
