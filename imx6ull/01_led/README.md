# QT LED

设置窗口大小：

```cpp
    QList <QScreen *> list_screen =  QGuiApplication::screens();
    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
    /* 默认是出厂系统的LED心跳的触发方式,想要控制LED，
     * 需要改变LED的触发方式，改为none，即无 */
    system("echo none > /sys/class/leds/red/trigger");
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(800, 480);
#endif
```