# ASR 百度语音识别模块

## 配置说明

### 1. 创建配置文件

在首次使用前，需要配置百度语音API密钥：

```bash
# 复制示例配置文件
cp config.ini.example config.ini
```

### 2. 填写API密钥

编辑 `config.ini` 文件，填入您从百度语音开放平台申请的密钥：

```ini
[BaiduASR]
api_key=YOUR_API_KEY_HERE
secret_key=YOUR_SECRET_KEY_HERE
```

### 3. 获取API密钥

1. 访问 [百度语音开放平台](https://console.bce.baidu.com/ai/#/ai/speech/overview/index)
2. 登录并创建应用
3. 获取 API Key 和 Secret Key
4. 将密钥填入 `config.ini` 文件

## 安全说明

- ⚠️ **重要**: `config.ini` 文件包含敏感信息，已添加到 `.gitignore` 中，不会被提交到Git仓库
- ✅ `config.ini.example` 是模板文件，会被提交到仓库中，但不包含真实密钥
- 🔒 请妥善保管您的API密钥，不要将其公开或提交到公共仓库

## 使用说明

配置完成后，程序会自动从 `config.ini` 读取API密钥。如果配置文件不存在或密钥为空，程序会在运行时输出警告信息。

配置文件路径默认为：`[应用程序目录]/config.ini`
