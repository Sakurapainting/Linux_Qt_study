# porting QT

## 移植QT

### 编译tslib

修改解压得到的 tslib-1.21 目录所属用户为当前用户，这一步一定要做

```bash
sudo chown you:you tslib-1.21 -R
```

```bash
sudo apt-get update
sudo apt-get install autoconf automake libtool m4 pkg-config
```

在tslib-1.21

执行 autogen.sh 生成 Makefile，以编译源码。

```bash
./autogen.sh
```

```bash
./configure --host=arm-linux-gnueabihf ac_cv_func_malloc_0_nonnull=yes --cache-file=arm-linux.cache -prefix=/home/you/linux/tool/tslib-1.21/arm-tslib
```

```bash
make
make install
```

安装完成后在当前目录下生成一个 arm-tslib 目录。也是我们上面指令的编译输出目录路径。

查看编译的 tslib 文件类型，使用 file 指令。编译出来的 tslib 应为 ARM 格式，不能为 X86格式，如果是 X86 格式就不能用于 ARM 开发板上了。

```bash
file bin/ts_calibrate
```

https://download.qt.io/archive/qt/5.12/5.12.9/single/

```bash
tar xf qt-everywhere-src-5.12.9.tar.xz
```

编辑 mkspecs/qws/linux-arm-gnueabi-g++/qmake.conf，配置相关编译器及路径。

```bash
nano qtbase/mkspecs/linux-arm-gnueabi-g++/qmake.conf
```

```conf

# qmake configuration for building with arm-linux-gnueabi-g++
#

MAKEFILE_GENERATOR      = UNIX
CONFIG                 += incremental
QMAKE_INCREMENTAL_STYLE = sublib

QT_QPA_DEFAULT_PLATFORM = linuxfb
QMAKE_CFLAGS += -O2 -march=armv7-a -mtune=cortex-a7 -mfpu=neon -mfloat-abi=hard
QMAKE_CXXFLAGS += -O2 -march=armv7-a -mtune=cortex-a7 -mfpu=neon -mfloat-abi=hard

include(../common/linux.conf)
include(../common/gcc-base-unix.conf)
include(../common/g++-unix.conf)

# modifications to g++.conf
QMAKE_CC                = arm-linux-gnueabihf-gcc
QMAKE_CXX               = arm-linux-gnueabihf-g++
QMAKE_LINK              = arm-linux-gnueabihf-g++
QMAKE_LINK_SHLIB        = arm-linux-gnueabihf-g++

# modifications to linux.conf
QMAKE_AR                = arm-linux-gnueabihf-ar cqs
QMAKE_OBJCOPY           = arm-linux-gnueabihf-objcopy
QMAKE_NM                = arm-linux-gnueabihf-nm -P
QMAKE_STRIP             = arm-linux-gnueabihf-strip
load(qt_config)
```

autoconfigure.sh

```sh
./configure -prefix /home/you/qt-everywhere-src-5.12.9/arm-qt \
-opensource \
-confirm-license \
-release \
-strip \
-shared \
-xplatform linux-arm-gnueabi-g++ \
-optimized-qmake \
-c++std c++11 \
--rpath=no \
-pch \
-skip qt3d \
-skip qtactiveqt \
-skip qtandroidextras \
-skip qtcanvas3d \
-skip qtconnectivity \
-skip qtdatavis3d \
-skip qtdoc \
-skip qtgamepad \
-skip qtlocation \
-skip qtmacextras \
-skip qtnetworkauth \
-skip qtpurchasing \
-skip qtremoteobjects \
-skip qtscript \
-skip qtscxml \
-skip qtsensors \
-skip qtspeech \
-skip qtsvg \
-skip qttools \
-skip qttranslations \
-skip qtwayland \
-skip qtwebengine \
-skip qtwebview \
-skip qtwinextras \
-skip qtx11extras \
-skip qtxmlpatterns \
-make libs \
-make examples \
-nomake tools -nomake tests \
-gui \
-widgets \
-dbus-runtime \
--glib=no \
--iconv=no \
--pcre=qt \
--zlib=qt \
-no-openssl \
--freetype=qt \
--harfbuzz=qt \
-no-opengl \
-linuxfb \
--xcb=no \
-tslib \
--libpng=qt \
--libjpeg=qt \
--sqlite=qt \
-plugin-sql-sqlite \
-I/home/you/linux/tool/tslib-1.22/arm-tslib/include \
-L/home/you/linux/tool/tslib-1.22/arm-tslib/lib \
-recheck-all
```

```bash
chmod +x autoconfigure.sh
sudo apt-get install g++
./autoconfigure.sh
```

如果是 Ubuntu18 以上，可能出现 C++11 Error, 解决方法是出现这种错误后，重新解压一次 Qt 源码，重新配置就不会出现 c++11 Error

我在编译时出现了其他问题，编译过程中无法识别 std::numeric_limits，这通常是因为缺少必要的头文件包含或编译器对 C++ 标准的支持不足。

std::numeric_limits 定义在 C++ 标准头文件 <limits> 中，错误提示 “‘numeric_limits’ is not a member of ‘std’” 说明代码中未包含该头文件，导致编译器无法识别。

需要手动在报错的文件中添加 limits 头文件包含

```bash
time (make -j 16)
# -j 16 代表最多允许 16 条编译指令同时运行，参数 16，一般为个人分配给虚拟机的核心数的 2 倍，前面加个 time 是测试编译时间。
```

```bash
make install
ls arm-qt
```

```bash
make[4]: 离开目录“/home/you/qt-everywhere-src-5.12.9/qtcharts/examples/charts/datetimeaxis”
/home/you/qt-everywhere-src-5.12.9/qtbase/bin/qmake -install qinstall /home/you/qt-everywhere-src-5.12.9/qtcharts/examples/charts/charts.pro /home/you/qt-everywhere-src-5.12.9/arm-qt/examples/charts/charts.pro
make[3]: 离开目录“/home/you/qt-everywhere-src-5.12.9/qtcharts/examples/charts”
make[2]: 离开目录“/home/you/qt-everywhere-src-5.12.9/qtcharts/examples”
make[1]: 离开目录“/home/you/qt-everywhere-src-5.12.9/qtcharts”
you@you-virtual-machine:~/qt-everywhere-src-5.12.9$ cd arm-qt/
you@you-virtual-machine:~/qt-everywhere-src-5.12.9/arm-qt$ ls
bin  doc  examples  include  lib  mkspecs  plugins  qml
you@you-virtual-machine:~/qt-everywhere-src-5.12.9/arm-qt$ 
```

编译安装好的 arm-tslib 文件夹打包成 tar.bz2 格式，打包以防
止文件丢失。

```bash
tar -jcf ./arm-tslib.tar.bz2 arm-tslib
```

解压到开发板的 /usr/lib 里

```bash
tar xf arm-tslib.tar.bz2 -C /usr/lib
```

由于上面烧写的文件系统已经移植过 tslib，已经在/etc/profile 文件配置过 tslib 的环境变量。所以我们的要重新配置我们自己的 tslib 环境变量，指定我们编译的 tslib 所在文件夹。避免和文件系统已经存在的 tslib 冲突。

编辑/etc/profile 文件，修改成以下内容。如下图红色框部分。“export TSLIB_CALIBFILE=/etc/pointercal”这项是电阻屏专用，如果是电容屏，可不用加这项。电容屏加了这项，如果使用 ts_calibrate 校准后会生成/etc/pointercal 文件，请把它删除！否则可以触摸不准确，因为电容屏不需要校准。注意，下面指令已经添加了 export LD_PRELOAD=$TSLIB_ROOT/lib/libts.so。

```bash
export TSLIB_ROOT=/usr/lib/arm-tslib
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export TSLIB_TSDEVICE=/dev/input/event1
export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts
export TSLIB_CALIBFILE=/etc/pointercal
export LD_PRELOAD=$TSLIB_ROOT/lib/libts.so
```

改动后的 /etc/profile ：

```bash
#!/bin/sh
LD_LIBRARY_PATH=/lib:/usr/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

export TERM=vt100
export TERMINFO=/usr/share/terminfo

# export TSLIB_TSDEVICE=/dev/input/event1
# export TSLIB_CALIBFILE=/etc/pointercal
# export TSLIB_CONFFILE=/etc/ts.conf
# export TSLIB_PLUGINDIR=/lib/ts
# export TSLIB_CONSOLEDEVICE=none
# export TSLIB_FBDEVICE=/dev/fb0

export TSLIB_ROOT=/usr/lib/arm-tslib                 
export TSLIB_CONSOLEDEVICE=none                      
export TSLIB_FBDEVICE=/dev/fb0                       
export TSLIB_TSDEVICE=/dev/input/event1
export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts
export LD_PRELOAD=$TSLIB_ROOT/lib/libts.so
                                          
export ALSA_CONFIG_PATH=/usr/share/arm-alsa/alsa.conf
```

测试tslib

```bash
source /etc/profile
# 使能环境变量
/usr/lib/arm-tslib/bin/ts_test
# 运行 ts_test 测试触摸是否正常，点击界面的 Draw 测试
```

打包qt

```bash
tar -jcf ./arm-qt.tar.bz2 arm-qt
```

解压qt

```bash
tar xf arm-qt.tar.bz2 -C /usr/lib
```

编辑/etc/profile，在末尾添加以下内容。要想 Qt 程序显示中文，请自行将 windows 下的（路径 C:\Windows\Fonts）下的中文字库放到新建一个/usr/share/fonts/目录下就可以了。注意 Windwos 的字库仅为个人学习使用，不要用于商业用途

```bash
export QT_ROOT=/usr/lib/arm-qt
export QT_QPA_GENERIC_PLUGINS=tslib:/dev/input/event1
export QT_QPA_FONTDIR=/usr/share/fonts
export QT_QPA_PLATFORM_PLUGIN_PATH=$QT_ROOT/plugins
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
export QT_PLUGIN_PATH=$QT_ROOT/plugins
export LD_LIBRARY_PATH=$QT_ROOT/lib:$QT_ROOT/plugins/platforms
export QML2_IMPORT_PATH=$QT_ROOT/qml
export QT_QPA_FB_TSLIB=1
```

改完的 /etc/profile ：

```bash
export TERMINFO=/usr/share/terminfo           
                                              
# export TSLIB_TSDEVICE=/dev/input/event1
# export TSLIB_CALIBFILE=/etc/pointercal 
# export TSLIB_CONFFILE=/etc/ts.conf     
# export TSLIB_PLUGINDIR=/lib/ts         
# export TSLIB_CONSOLEDEVICE=none        
# export TSLIB_FBDEVICE=/dev/fb0         
                                        
export TSLIB_ROOT=/usr/lib/arm-tslib
export TSLIB_CONSOLEDEVICE=none     
export TSLIB_FBDEVICE=/dev/fb0      
export TSLIB_TSDEVICE=/dev/input/event1
export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts    
export LD_PRELOAD=$TSLIB_ROOT/lib/libts.so                                                
export QT_ROOT=/usr/lib/arm-qt               
export QT_QPA_GENERIC_PLUGINS=tslib:/dev/input/event1
export QT_QPA_FONTDIR=/usr/share/fonts               
export QT_QPA_PLATFORM_PLUGIN_PATH=$QT_ROOT/plugins  
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0          
export QT_PLUGIN_PATH=$QT_ROOT/plugins               
export LD_LIBRARY_PATH=$QT_ROOT/lib:$QT_ROOT/plugins/platforms
export QML2_IMPORT_PATH=$QT_ROOT/qml                          
export QT_QPA_FB_TSLIB=1

export ALSA_CONFIG_PATH=/usr/share/arm-alsa/alsa.conf 
```

测试 Qt 运行

使能 Qt 的配置在/etc/profile 的环境变量，下次开机不用使能，开机会自动使能这个环境变量。

```bash
source /etc/profile
/usr/lib/arm-qt/examples/widgets/animation/animatedtiles/animatedtiles #运行编译的示例
```
## 配置 QT Creator 交叉编译

打开 Qt Creator 界面，找到 Tools（工具）-> Options（选项）

Kits -> Qt Versions -> 手动设置 -》 /`home/you/qt-everywhere-src-5.12.9/arm-qt/bin/qmake`

### 配置 C++编译器

Kits -> 编译器 -> 添加 -> GCC -> C++ -> 改名为 "Arm-GCC" -> 浏览选择下面的路径

`/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++`

### 配置 Kits

Kits -> 手动设置 -> Add -> 改名为 "Arm-Qt5.12.9" -> C: no compiler -> C++: Arm-GCC -> Qt Versions: Qt 5.12.9(arm-qt)

### 验证

注意，这里只能编译(构建)，不能点击运行

放到开发板 /usr/bin 里

给运行权限后运行

## 命令行编译 Qt 工程

可以直接在 test 工程文件夹里，直接运行 qmake，生成 Makefile，再执行 make 开始编译。

```bash
cd test
# 进入 test 工程文件夹下
/home/you/qt-everywhere-src-5.12.9/arm-qt/bin/qmake # qmake 的绝对路径
make -j16
```

编译完成会在当前文件夹生成 test 文件，拷贝到开发板文件系统下直接使用 ./test 执行即可。