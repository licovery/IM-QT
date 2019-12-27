#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "login.h"
#include "mainwindow.h"

#include "im.pb.h"

class Client: public QObject
{
    Q_OBJECT
public:
    enum Status
    {
        DISCONNECTED = 0,
        CONNECTED = 1,
    };

    Client(QObject *parent = nullptr);
    ~Client() override;

    bool connectToServer();
    void showNetworkError() const;

    void startLogin();
    void closeLogin();
    void startMainUi();
    void closeMainUi();

    void sendData(const std::string &buf);

    void recvLoginRspProc(const LoginResponse &loginRsp);
    void recvOnlineRspProc(const OnlineUserResponse &onelineRsp);
    void recvMsgProc(const InstantMessaging &chatMsg);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    Login *loginUi;
    MainWindow *mainUi;
    QTcpSocket *tcpSocket;
    QString clientName;
    Status networkStatus;

    int OnlineUserFlushtimer; // 定时器，用于刷新在线用户

public slots:
    void recvData();
    void serverClose();
    // 发送请求
    void login(QString userId);
    void searchOnlineUser();
    void sendChatMsg(QString dstId, QString content);
//    void logout();
};

#endif // CLIENT_H
