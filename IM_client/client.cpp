#include "client.h"
#include "im.pb.h"
#include "msg.h"
#include <QMessageBox>
#include <QTimerEvent>

Client::Client(QObject *parent):
    QObject (parent), loginUi(nullptr), mainUi(nullptr),
    tcpSocket(new QTcpSocket), networkStatus(DISCONNECTED)
{

}

Client::~Client()
{
    if (!tcpSocket) delete tcpSocket;
    if (!loginUi) delete loginUi;
    if (!mainUi) delete  mainUi;
}

bool Client::connectToServer()
{
    tcpSocket->connectToHost("127.0.0.1", 8888);
    // 等待最长不超过1000ms
    tcpSocket->waitForConnected(1000);
    if (tcpSocket->state() != tcpSocket->ConnectedState)
    {

        tcpSocket->close();
        networkStatus = DISCONNECTED;
        showNetworkError();
        return false;
    }

    // 连接成功，并处理连接被中断的情况
    networkStatus = CONNECTED;
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(recvData()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(serverClose()));
    return true;
}

void Client::startLogin()
{

    if (loginUi == nullptr)
    {
        loginUi = new Login();
    }
    loginUi->show();

    connect(loginUi, SIGNAL(userLogin(QString)), this, SLOT(login(QString)));
}

void Client::closeLogin()
{
    if (loginUi != nullptr)
    {
        loginUi->close();
        delete loginUi;
    }
}

void Client::startMainUi()
{
    if (mainUi == nullptr)
    {
        mainUi = new MainWindow(clientName);
    }
    mainUi->show();
    // 定时刷新在线用户
    OnlineUserFlushtimer = QObject::startTimer(20000);
    connect(mainUi, SIGNAL(sendMessage(QString, QString)), this, SLOT(sendChatMsg(QString, QString)));
}

void Client::closeMainUi()
{
    if (mainUi != nullptr)
    {
        mainUi->close();
        delete mainUi;
    }
}

void Client::sendData(const std::string &buf)
{
    if (networkStatus != CONNECTED)
    {
        showNetworkError();
        return;
    }

    if (tcpSocket->isWritable())
    {
        tcpSocket->write(buf.c_str(), buf.size());
    }
    else
    {
        showNetworkError();
        qDebug() << "can not write";
        return;
    }
}

void Client::recvLoginRspProc(const LoginResponse &loginRsp)
{
    auto result = MsgParser::parseLoginRsp(loginRsp);
    // 校验id字符串
    if (result == LoginResponse::SUCC)
    {
        qDebug() << "login successful" << endl;
        closeLogin();
        startMainUi();
        searchOnlineUser();
    }
    else if (result == LoginResponse::ID_CONFLICT)
    {
        QMessageBox::warning(loginUi, "login error", "id conflict, please change your id.");
    }
    else
    {
        QMessageBox::warning(loginUi, "unknow error", "unknow");
    }
}

void Client::recvOnlineRspProc(const OnlineUserResponse &onelineRsp)
{
    auto users = MsgParser::parseOnlineUsers(onelineRsp);
    mainUi->printOnlineUserInfo(users);
}

void Client::recvMsgProc(const InstantMessaging &chatMsg)
{
    auto msg = MsgParser::parseChatMsg(chatMsg);
    mainUi->printRecvMsg(chatMsg.from_user_id(), msg);
}

void Client::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == OnlineUserFlushtimer)
    {
        searchOnlineUser();
    }
}

void Client::showNetworkError() const
{
    qDebug() << tcpSocket->state();
    QMessageBox::critical(nullptr, "Network Error",
                          "Can not connect to server, please check your network config and try agign !");
    qDebug() << "connect to server error";
}

void Client::recvData()
{
    if (networkStatus != CONNECTED)
    {
        showNetworkError();
        return;
    }

    if (tcpSocket->isReadable())
    {
        auto rsp = tcpSocket->readAll();
        Interface pbMsg;
        pbMsg.ParseFromString(rsp.toStdString());
        if (pbMsg.interface_case() == Interface::kLoginRsp)
        {
            recvLoginRspProc(pbMsg.login_rsp());
        }
        else if (pbMsg.interface_case() == Interface::kOnlineRsp)
        {
            recvOnlineRspProc(pbMsg.online_rsp());
        }
        else if (pbMsg.interface_case() == Interface::kMsg)
        {
            recvMsgProc(pbMsg.msg());
        }
    }
    else
    {
        qDebug() << "login: nothing to read";
    }
}

void Client::serverClose()
{
    tcpSocket->close();
    networkStatus = DISCONNECTED;
    showNetworkError();

    // 关闭界面, 可以改为重新连接
    closeLogin();
    closeMainUi();
}

void Client::login(QString userId)
{
    // 首次记录客户ID
    clientName = userId;
    std::string loginReq(MsgBuilder::buildLoginReq(clientName.toStdString()));
    sendData(loginReq);
}

void Client::searchOnlineUser()
{
    std::string onlineReq = MsgBuilder::buildOnlineUsersReq(clientName.toStdString());
    sendData(onlineReq);
}

void Client::sendChatMsg(QString dstId, QString content)
{
    std::string msg = MsgBuilder::buildChatMsg(content.toStdString(),
                      clientName.toStdString(),
                      dstId.toStdString());
    sendData(msg);
}


