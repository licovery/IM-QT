#include "register.h"
#include "ui_register.h"
#include <QMessageBox>

Register::Register(int sockfd, QWidget *parent) :
   sockfd(sockfd),QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    connect(ui->btn_commit,SIGNAL(clicked()),this,SLOT(submitTo()));
}

Register::~Register()
{
    delete ui;
}

void Register::submitTo()
{
    registerInfo rf;
    memset(&rf,0,sizeof(rf));
    rf.flag = 2;
    if(ui->lineEdit_name->text()==""||ui->lineEdit_pwd->text()=="")//用户名或者密码为空则返回
    {
        QMessageBox::warning(this,"register","empty id or password");
        return;
    }
    strcpy(rf.name,ui->lineEdit_name->text().toStdString().c_str());
    strcpy(rf.pwd,ui->lineEdit_pwd->text().toStdString().c_str());
    send(sockfd,&rf,sizeof(rf),0);
    char account[20] = {0};
    recv(sockfd,account,sizeof(account),0);
    QMessageBox::information(this,"register","successful");
    close();
}
