# Qt camera

## 正点原子imx6ull资源

正点原子 I.MX6U 开发板底板上有一路“CSI”摄像头接口。支持正点原子的 OV5640、OV2640 和 OV7725(不带 FIFO)。同时有 USB 接口，可以接 USB 免驱摄像头。

## 环境搭建

Qt 里也有一个 QCamera 类。没错，确实可以使用这个 QCamera 类来开发摄像头。但是这个类在正点原子的 I.MX6U 开发板 4.1.15 内核版本上不能使用 OV5640、OV2640 和 OV7725，可以使用 USB 免驱摄像头。因为 OV5640、OV2640 和 OV7725 的驱动默认是读取 YUYV 格式数据，而 QCamera 里读取的数据是 RGB 格式数据，它们可能数据对不上就无法使用了！但是也不建议修改驱动的方法来使用 QCamera，防止 QCamera 在某些方法上与驱动层不对应，导致使用报错。

实际上我们使用 V4l2 编程就可以对摄像头进行编程，效果会比在 Qt 显示流畅，因为 V4l2的数据直接可以显示在 fb0(也就是屏上)。而经过 Qt 还需要在控件上处理，所以效率会慢一些

Qt + OpenCV 调用摄像头，效果肯定不能与使用 V4l2 直接显示在 fb0 上相比。在这个开发板的 CPU 上显示效果还是比较好的，还能接受，流畅度一般。这个可能是 OpenCV在对摄像头数据做了一定的处理才会变的慢了。

```bash
tar xf opencv-3.4.1.tar.gz
cd opencv-3.4.1
```

```bash
sudo apt-get install cmake
```

```bash
mkdir build
cd build
```

执行 cmake 配置编译。这里表示上一层目录。cmake 会从上一层目录下找配置项，并配置到当前目录。

```bash
cmake ..
```

在配置的过程中 cmake 会下载一些库，如 ippicv_2017u3_lnx_intel64_general_20170822.tgz

配置成功有如下信息：

```bash
-- -----------------------------------------------------------------
-- 
-- Configuring done
-- Generating done
-- Build files have been written to: /home/you/opencv-3.4.1/build
```

开始编译

```bash
make -j16
```

编译中间碰到了权限问题，给sudo可继续。

还有，OpenCV 3.4.1 内置的 OpenEXR 库使用了 C++17 已废弃的动态异常规范（throw(...)），而ubuntu编译器默认启用了 C++17 标准，导致不兼容。

需要让编译器使用 C++14 或更低标准（避免 C++17 对动态异常规范的限制）

重新配置 CMake，添加 C++ 标准参数：

```bash
# 清理之前的编译缓存
make clean
rm -rf CMakeCache.txt CMakeFiles/

# 配置时指定 C++ 标准为 C++14
cmake -DCMAKE_CXX_STANDARD=14 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..

# 重新编译
make -j$(nproc)
```

若仍有问题，可尝试更低的标准（如 CMAKE_CXX_STANDARD=11）。但是这里降为C++14就能继续编译了。

编译完成后，安装

```bash
sudo make install
```

可以看到库被安装到/usr/local/lib 下，头文件被安装在/usr/local/include 下

我们只需要知道安装的库路径和头文件路径即可在 Qt 里调用 Ubuntu 安装的 OpenCV。头文件作用来编写程序，库路径用来运行程序时调用。我们只要在 Qt 的 pro 项目文件里指定这两个路径即可。

## imx6ull 移植opencv

```bash
cd opencv-3.4.1/
mkdir build-arm install
sudo apt-get install cmake cmake-qt-gui cmake-curses-gui
```

```bash
cd build-arm
cmake-gui
```

指定我们源码的所在路径和构目录。

source code: `/home/you/opencv-3.4.1`
build the binaries: `/home/you/opencv-3.4.1/build-arm`

选中 Grouped 和 Advanced ，然后 Generate

选择 Unix Makefiles，然后选择 Specify options for cross-compiling，再点击 Next。

填写交叉编译器的路径

Operation System: `Linux`
Compilers - C: `/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc`
Compilers - C++: `/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++`
Find Program/Library/Include - Target Root: `/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/`
Find Program/Library/Include - Program Mode: `Search in Target Root, then native system`
Find Program/Library/Include - Library Mode: `Search only in Target Root`
Find Program/Library/Include - Include Mode: `Search only in Target Root`

下面就是您配置的信息，可以配置很多项，比如要编译哪些库等都可以在此选择编译或者不编译。比如常见的 V4L2 都需要自己打开

点击 CMAKE，在 CMAKE_EXE_LINKER_FLAGS 处添加上“-lpthread -lrt -ldl”添加这些是指定依赖库的链接参数。上面的“-lxx”其中“-l”是大写字母“L”的小写字母“l”，不是数字“1”。

再在 CMAKE_INSTALL_PREFIX 处指定安装目录，我们在上面已经新建了 install 安装目录。如果不指定，它会默认安装到 Ubuntu 系统目录/usr/local 下。再点击关闭。到这里我们已经完成配置

至此我们配置完，现在我们需要生成 Makefile 等文件，我们先点击 Configure，再点击 Generate 就可以生成了。

首先在源码目录 3rdparty/protobuf/src/google/protobuf/stubs/common.cc 这个文件下第 33 行添加#define HAVE_PTHREAD 宏定义才可以编译的过。具体原因是 HAVE_PTHREAD 宏定义了 pthread 库。

```bash
cd ..   # 返回 opencv 源码顶层目录
vi 3rdparty/protobuf/src/google/protobuf/stubs/common.cc
```

在注释下方，include上方加入

```c
#define HAVE_PTHREAD
```

```bash
cd build-arm/
make -j16
```

```bash
make install
ls ../install
```

拷贝到 开发板 /usr/lib 下

## 编写应用

```bash
/opt/Qt5.12.9/Tools/QtCreator/bin/qtcreator.sh &
```

在项目 pro 文件添加 OpenCV 库的支持及头文件路径。判断交叉编译器的类型，然后链接到不同的头文件路径与库。

```makefile
TARGET_ARCH = $${QT_ARCH}
contains(TARGET_ARCH, arm){
    CONFIG += link_pkgconfig
    QMAKE_PKGCONFIG_LIBDIR = /home/you/opencv-3.4.1/install/lib/pkgconfig
    PKGCONFIG += opencv
    INCLUDEPATH += /home/you/opencv-3.4.1/install/include
} else {
    LIBS += -L/home/you/opencv-3.4.1/install/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_videoio \
            -lopencv_imgcodecs

    #INCLUDEPATH可写可不写，系统会到找到此路径
    INCLUDEPATH += /usr/local/include
}
```

但是实际上，我的测试环境使用pkgconfig会出错，使用下面的配置不会出错

```makefile
TARGET_ARCH = $${QT_ARCH}
contains(TARGET_ARCH, arm){
    INCLUDEPATH += /home/you/opencv-3.4.1/install/include
    LIBS += -L/home/you/opencv-3.4.1/install/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_videoio \
            -lopencv_imgcodecs
}
```

INCLUDEPATH 是 arm 对应 opencv 的头文件路径，可以不写，编译不会报错，但是我们想查看对应的头文件，就不得不包括这个路径了，否则跳转不过去

LIBS 是 添加库的支持。-L 后面指的是库文件路径，-l 后面的是相关库参数（l 是大字母“L”的小写字母“l”，不是一）

camera.h

前向声明 告诉编译器 cv::VideoCapture 和 cv::Mat 是存在的类

Camera类的构造函数需要使用explicit，防止隐式类型转换，强制必须显式调用构造函数。

camera.cpp

```
定时器触发(33ms) 
    ↓
capture >> frame (读取一帧)
    ↓
matToQImage(frame) (格式转换 + RGB↔BGR)
    ↓
emit readyImage(QImage) (发射信号)
    ↓
外部 UI 接收并显示
```

```cpp
if(img.type() == CV_8UC3)  // 检查是否为 8位无符号3通道图像
```

- CV_8UC3 = 8-bit Unsigned Char, 3 Channels
- 即 RGB/BGR 彩色图像，每个像素 3 个字节

```cpp
QImage qImage(pimg,        // 数据指针
              img.cols,    // 图像宽度（像素）
              img.rows,    // 图像高度（像素）
              img.step,    // 每行字节数（含填充）
              QImage::Format_RGB888);  // 像素格式
```

```
cv::Mat (BGR格式)
    ↓
获取 img.data 指针 (零拷贝)
    ↓
QImage 构造 (共享内存，标记为RGB888)
    ↓
rgbSwapped() (通道语义交换)
    ↓
返回正确的 QImage (RGB格式) ✅
```

mainwindow.cpp

```
┌─────────────────────────────────────┐
│         MainWindow (800×480)        │
├──────────────────┬──────────────────┤
│   ScrollArea     │  Right Widget    │
│                  │                  │
│   DisplayLabel   │  ┌─────────────┐ │
│   (实时视频)      │  │ PhotoLabel  │ │
│                  │  │  (缩略图)    │ │
│                  │  ├─────────────┤ │
│                  │  │ ComboBox    │ │
│                  │  │ (设备选择)   │ │
│                  │  ├─────────────┤ │
│                  │  │ 拍照按钮     │ │
│                  │  ├─────────────┤ │
│                  │  │ 开始/关闭    │ │
│                  │  └─────────────┘ │
└──────────────────┴──────────────────┘
    HBoxLayout            VBoxLayout
```

```
用户点击"开始"
    ↓
selectCameraDevice(index) → 打开摄像头
    ↓
cameraProcess(true) → 启动定时器(33ms)
    ↓
[循环] Camera::timerTimeOut
    ↓
emit readyImage(QImage)
    ↓
MainWindow::showImage
    ↓
displayLabel 显示实时画面
    ↓
用户点击"拍照"
    ↓
saveImageToLocal → 保存 PNG 文件
    ↓
photoLabel 显示缩略图 ✅
```

不要选择 video0，video0 是 NXP 的 pxp 驱动产生的节点，不是摄像头

免驱摄像头没有识别到

去menuconfig 用 / 搜索 UVC，把这些的内核驱动都打开，打开4+1个，下面只贴出一个的内容

```
Symbol: USB_CONFIGFS_F_UVC [=n]                                                                                                                                         │  
  │ Type  : boolean                                                                                                                                                         │  
  │ Prompt: USB Webcam function                                                                                                                                             │  
  │   Location:                                                                                                                                                             │  
  │     -> Device Drivers                                                                                                                                                   │  
  │       -> USB support (USB_SUPPORT [=y])                                                                                                                                 │  
  │         -> USB Gadget Support (USB_GADGET [=y])                                                                                                                         │  
  │           -> USB Gadget Drivers (<choice> [=m])                                                                                                                         │  
  │ (1)         -> USB functions configurable through configfs (USB_CONFIGFS [=m])                                                                                          │  
  │   Defined at drivers/usb/gadget/Kconfig:429                                                                                                                             │  
  │   Depends on: <choice> && USB_CONFIGFS [=m] && VIDEO_DEV [=y]                                                                                                           │  
  │   Selects: VIDEOBUF2_VMALLOC [=y] && USB_F_UVC [=n]    
```

然后内核勾选了UVC

```bash
/ # ls -l /dev/video*
crw-rw----    1 root     0          81,   0 Jan  1 00:00 /dev/video0
crw-rw----    1 root     0          81,   1 Jan  1 02:46 /dev/video1
```

选择video1，拍照很慢，且保存到可执行文件的目录下 test.png，多次拍照会覆盖