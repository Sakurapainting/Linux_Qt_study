# Qt 学习项目

这是一个基于Qt 5.12.9的GUI应用程序开发学习项目。

## 开发环境

### 我的开发平台
- **Qt版本**: Qt 5.12.9
- **编译器**: GCC 64-bit (Linux)
- **IDE**: Qt Creator / VS Code
- **操作系统**: Ubuntu Linux
- **Qt安装路径**: `/opt/Qt5.12.9`

## 项目结构

```
Qt/
├── .vscode/                        # VS Code配置文件
│   ├── c_cpp_properties.json       # C++智能感知配置
│   └── settings.json               # 工作区设置
├── .gitignore                      
├── README.md                       # 项目说明文档
├── xxx_xxxxxx/                     # 第xxx章：xxxxxx
│   ├── xxx_xxxxxx.pro              # Qt项目文件
│   ├── main.cpp                    # 主程序入口
│   ├── mainwindow.cpp/h            # 主窗口实现
│   ├── mainwindow.ui               # UI设计文件
│   └── README.md                   # 章节笔记
```

## 使用方法

### 使用Qt Creator

#### 1. 打开项目
```bash
# 启动Qt Creator
qtcreator

# 或直接打开项目文件
qtcreator 01_hello_world/01_hello_world.pro
```

#### 2. 配置构建套件（Kit）
首次打开项目时，Qt Creator会提示配置Kit：
- 选择：Desktop Qt 5.12.9 GCC 64bit
- 点击"Configure Project"

#### 3. 编译运行
- **编译**: 点击左下角的🔨图标，或按 `Ctrl+B`
- **运行**: 点击左下角的▶️图标，或按 `Ctrl+R`
- **调试**: 点击左下角的🐛图标，或按 `F5`

## 贡献指南

欢迎提交Issue和Pull Request来改进这个项目：
- Bug修复
- 代码优化
- 文档改进
- 新示例添加

### PR规则
请向 `dev` 分支合并，不要向 `main` 提 PR。

## 许可证

本项目采用MIT许可证，详见LICENSE文件。

## 联系方式

如有问题请提交Issue或发送邮件讨论。

---

**注意**: 此项目仅用于学习目的，代码仅作为示例参考。
