/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   asr
* @brief         asr.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-03
*******************************************************************/
#ifndef ASR_H
#define ASR_H

#include <QWidget>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#ifndef QT_NO_SSL
#include <QSslConfiguration>
#include <QSslSocket>
#endif

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostInfo>

#include <QFile>
#include <QSettings>

class Asr : public QWidget
{
    Q_OBJECT

public:
    Asr(QWidget *parent = nullptr);
    ~Asr();

    /* 请求网络 */
    void requestNetwork(QString, QByteArray);

    /* 获取识别结果 */
    void getTheResult(QString fileName);

private:
    /* 存储获取tokenUrl地址 */
    QString tokenUrl;

    /* 存储serverapi地址 */
    QString serverApiUrl;

    /* 最终需要访问token的地址 */
    QString accessToken;

    /* 获取token的接口*/
    const QString token_org = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";

    /* API密钥从配置文件读取 */
    QString api_key;
    QString secret_key;

    /* 百度服务器API接口，发送语音可返回识别结果 */
    const QString server_api = "http://vop.baidu.com/server_api?dev_pid=1537&cuid=%1&token=%2";

    /* 网络管理 */
    QNetworkAccessManager *networkAccessManager;

    QString getJsonValue(QByteArray ba, QString key);
    
    /* 从配置文件加载API密钥 */
    void loadConfig();

    QFile file;

private slots:

    /* 准备读取响应返回来的数据 */
    void readyReadData();

    /* 响应完成处理 */
    void replyFinished();

signals:
    void asrReadyData(QString);

};
#endif // ASR_H
