#include "login.h"
#include "ui_login.h"
#include "msg.h"
#include <QMessageBox>
#include <utility>

Login::Login(QWidget *parent) :
    QDialog(parent), ui(new Ui::Login), main_face(nullptr), tcpSocket(new QTcpSocket)
{
    ui->setupUi(this);

    tcpSocket->connectToHost("127.0.0.1", 8888);
    // 等待最长不超过1000ms
    tcpSocket->waitForConnected(1000);
    if (tcpSocket->state() != tcpSocket->ConnectedState)
    {
        qDebug() << tcpSocket->state();
        tcpSocket->close();
        QMessageBox::critical(this, "Network Error",
                              "Can not connect to server, please check your network config and try agign !");
        qDebug() << "connect to server error";
    }

    connect(ui->btn_login, SIGNAL(clicked()), this, SLOT(loginTo()));

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(loginCheck()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(serverClose()));

//    connect(tcpSocket, SIGNAL(connected()), this, SLOT()); 提示连接成功
//    connect(ui->btn_register,SIGNAL(clicked()),this,SLOT(registerTo()));
}

Login::~Login()
{
    delete ui;
    delete tcpSocket;
}

void Login::loginTo()
{
    if (ui->lineEdit_id->text() == "")
    {
        QMessageBox::warning(this, "Login", "id or password is empty");
        return;
    }

    clientName = ui->lineEdit_id->text();

    std::string buf(MsgBuilder::buildLoginReq(clientName.toStdString()));

    if (tcpSocket->isWritable())
    {
        tcpSocket->write(buf.c_str(), buf.size());
    }
    else
    {
        QMessageBox::critical(this, "Network Error",
                              "Can not connect to server, please check your network config and try agign !");
        tcpSocket->close();
        return;
    }
}

void Login::loginCheck()
{
    if (tcpSocket->isReadable())
    {
        auto rsp = tcpSocket->readAll();
        Interface pbMsg;
        pbMsg.ParseFromString(rsp.toStdString());
        if (pbMsg.interface_case() == pbMsg.kLoginRsp)
        {
            if (pbMsg.login_rsp().result() == LoginResponse::SUCC)
            {
                qDebug() << "login successful" << endl;
                close();
                // 断开信号槽操作
                disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(loginCheck()));
                disconnect(tcpSocket, SIGNAL(disconnected()), this, SLOT(serverClose()));
                main_face = new MainInterface(tcpSocket, QString(clientName));
                main_face->show();
            }
            else if (pbMsg.login_rsp().result() == LoginResponse::ID_CONFLICT)
            {
                QMessageBox::warning(this, "login error", "id conflict, please change your id.");
            }
            else
            {
                QMessageBox::warning(this, "unknow error", "unknow");
            }
        }
        else
        {
            qDebug() << "login: read something unknow";
        }
    }
    else
    {
        qDebug() << "login: nothing to read";
    }
}



//void Login::registerTo()
//{
//    sockfd  = cs.connectTo("127.0.0.1",8888);
//    UserInfo userdata;
//    userdata.mode = REGISTER;
//    int flag;

//    if(ui->lineEdit_id->text()==""||ui->lineEdit_pwd->text()=="")//用户名或者密码为空则返回
//    {
//        QMessageBox::warning(this,"Register","empty id or password");
//        return;
//    }

//    strncpy(userdata.id, ui->lineEdit_id->text().toStdString().c_str(), sizeof(userdata.id));
//    strncpy(userdata.pwd,ui->lineEdit_pwd->text().toStdString().c_str(), sizeof(userdata.id));

//    send(sockfd,&userdata,sizeof(userdata),0);
//    int ret = recv(sockfd,&flag,sizeof(flag),0);
//    if (ret == 0 || ret == -1) {
//        qDebug() << "recv error";
//        return;
//    }

//    if (flag == 1)
//        QMessageBox::information(NULL,"Register","successful");
//    else
//        QMessageBox::warning(NULL,"Register","fail");

//    cs.closeSockfd(sockfd);
//}



// 暂时无法区分主动关闭或者服务器断开连接
void Login::serverClose()
{
    tcpSocket->close();
    QMessageBox::information(this, "Network close", "Close");
    this->close();
}

