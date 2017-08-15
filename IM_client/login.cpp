#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QDialog(parent), ui(new Ui::Login)
{

    ui->setupUi(this);

    connect(ui->btn_login,SIGNAL(clicked()),this,SLOT(loginTo()));
    connect(ui->btn_register,SIGNAL(clicked()),this,SLOT(registerTo()));
}

Login::~Login()
{
    delete ui;
}

void Login::loginTo()
{
    sockfd  = cs.connectTo("127.0.0.1",8888);
    UserInfo userdata;
    userdata.mode= LOGIN;
     int flag = 0;

    if(ui->lineEdit_id->text()==""||ui->lineEdit_pwd->text()=="")
    {
        QMessageBox::warning(this, "Login", "id or password is empty");
        return;
    }

    strncpy(userdata.id ,ui->lineEdit_id->text().toStdString().c_str(), sizeof(userdata.id));
    strncpy(userdata.pwd,ui->lineEdit_pwd->text().toStdString().c_str(), sizeof(userdata.pwd));


    send(sockfd,&userdata,sizeof(userdata),0);
    int ret = recv(sockfd,&flag,sizeof(int),0);

    if (ret == 0 || ret == -1) {
        qDebug() << "recv error";
        return;
    }

    if(flag == 1)
    {
        qDebug() << "login successful" << endl;
        hide();
        main_face = new interface(sockfd, QString(userdata.id));
        main_face->show();
    }
    else
    {
        QMessageBox::warning(this, "Login", "id or password is wrong");
        cs.closeSockfd(sockfd);
    }
}


void Login::registerTo()
{
    sockfd  = cs.connectTo("127.0.0.1",8888);
    UserInfo userdata;
    userdata.mode = REGISTER;
    int flag;

    if(ui->lineEdit_id->text()==""||ui->lineEdit_pwd->text()=="")//用户名或者密码为空则返回
    {
        QMessageBox::warning(this,"Register","empty id or password");
        return;
    }

    strncpy(userdata.id, ui->lineEdit_id->text().toStdString().c_str(), sizeof(userdata.id));
    strncpy(userdata.pwd,ui->lineEdit_pwd->text().toStdString().c_str(), sizeof(userdata.id));

    send(sockfd,&userdata,sizeof(userdata),0);
    int ret = recv(sockfd,&flag,sizeof(flag),0);
    if (ret == 0 || ret == -1) {
        qDebug() << "recv error";
        return;
    }

    if (flag == 1)
        QMessageBox::information(NULL,"Register","successful");
    else
        QMessageBox::warning(NULL,"Register","fail");

    cs.closeSockfd(sockfd);
}

