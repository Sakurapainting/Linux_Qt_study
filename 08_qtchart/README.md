# QChart

要想在项目里使用 Qt Charts 模块，需要在 pro 文件下添加以下语句。

```
QT += charts
```

还有命名空间, 一般在头文件处加上这个

```cpp
QT_CHARTS_USE_NAMESPACE
```

所以

```
QT       += core gui charts
```

本例基本流程如下：使用一个 QSplineSeries 对象（曲线），一个 QChart（图表），一个 QChartView（图表视图）。首先我们创建坐 chart 图表，然后创建两条坐标轴 axisX 与 axisY。将两条坐标轴添加到chart 图表上，再将 splineSeries 曲线与坐标轴连系起来。最后再将 chart 图表添加到 chartView图表视图中。曲线上的数据由系统产生随机数，使用定时器更新数据。

数据的个数超过最大值后，就删除第一个数据，如此反复，就实现了数据移动的过程，同时图表视图中的曲线因为值的改变实现了“移动”。

