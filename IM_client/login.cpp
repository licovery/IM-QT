#include "login.h"
#include "ui_login.h"
#include "msg.h"
#include <QMessageBox>
#include <utility>

Login::Login(QWidget *parent) :
    QDialog(parent), ui(new Ui::Login)
{
    ui->setupUi(this);

    // 用户主动触发信号
    connect(ui->btn_login, SIGNAL(clicked()), this, SLOT(loginTo()));
//    connect(ui->btn_register,SIGNAL(clicked()),this,SLOT(registerTo()));
}

Login::~Login()
{
    delete ui;
}


void Login::loginTo()
{

    if (ui->lineEdit_id->text() == "")
    {
        QMessageBox::warning(this, "Login", "Id is empty");
        return;
    }
    emit userLogin(ui->lineEdit_id->text());
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




