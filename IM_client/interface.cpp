#include "interface.h"
#include "ui_interface.h"
#include "file.h"

interface::interface(int sockfd, QString my_id, QWidget *parent) :
    sockfd(sockfd),my_id(my_id),QWidget(parent),
    ui(new Ui::interface)
{
    ui->setupUi(this);
    //default all mode
    chatType = 0;
    to_user_id = "";
    strcpy(msg.from_user_id,this->my_id.toStdString().c_str());
    ui->me->setText(my_id);

    ui->btn_all->setEnabled(false);

    ui->tableWidget_userlist->setColumnCount(1);//设置column为1
    ui->tableWidget_userlist->setColumnWidth(0,211);
    QStringList list;
    list << "userID";
    ui->tableWidget_userlist->setHorizontalHeaderLabels(list);
    ui->tableWidget_userlist->verticalHeader()->setVisible(false); //隐藏行表头
    ui->tableWidget_userlist->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
    ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_userlist->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改


    ui->tableWidget_msgPage->setColumnCount(1);
    ui->tableWidget_msgPage->setShowGrid(false);
    ui->tableWidget_msgPage->setColumnWidth(0,500);
    ui->tableWidget_msgPage->verticalHeader()->setVisible(false);
    ui->tableWidget_msgPage->horizontalHeader()->setVisible(false);
    ui->tableWidget_msgPage->setEditTriggers(QAbstractItemView::NoEditTriggers);

    recv_Online_userinfo();

    thread = new recvMsgThread(sockfd);
    thread->start();

    connect(ui->tableWidget_userlist,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));
    connect(ui->btn_one,SIGNAL(clicked()),this,SLOT(btn_one_slot()));
    connect(ui->btn_all,SIGNAL(clicked()),this,SLOT(btn_all_slot()));
    connect(ui->btn_send,SIGNAL(clicked()),this,SLOT(btn_send_slot()));
    connect(ui->btn_refresh,SIGNAL(clicked()),this,SLOT(btn_refresh_slot()));
    connect(ui->btn_send_file,SIGNAL(clicked()),this, SLOT(btn_send_file_slot()));
    connect(thread,SIGNAL(sendMsg_signal(Msg*)),this,SLOT(recv_msg(Msg*)));
    connect(thread,SIGNAL(refresh_signal(Msg*, int)),this,SLOT(recv_user(Msg*, int)));


}

interface::~interface()
{
    delete ui;
}

void interface::closeEvent(QCloseEvent *event)
{
    connectServer::closeSockfd(sockfd);
}



void interface::recv_Online_userinfo()
{
    int length = 0;
    recv(sockfd,&length,sizeof(int),0);
    qDebug() << "online user count is:" << length << endl;
    online_userinfo ouf;

    int i = 0;
    while(length--)
    {
        recv(sockfd,&ouf,sizeof(ouf),0);
        ui->tableWidget_userlist->insertRow(i);//刷新列表，删除原来的项，再重新从服务器获取数据
        ui->tableWidget_userlist->setItem(i,0,new QTableWidgetItem(QString(ouf.id)));
        i++;
        qDebug() << "userID: " << ouf.id << endl;
    }
}

void interface::getItem(QTableWidgetItem *item)
{
    if(1 == chatType)
    {
        int row_number = ui->tableWidget_userlist->currentRow();
        to_user_id = ui->tableWidget_userlist->item(row_number,0)->data(Qt::DisplayRole).toString();
        ui->chatuser->setText(to_user_id);
        strcpy(msg.to_user_id ,to_user_id.toStdString().c_str());
        ui->btn_send->setEnabled(true);
    }
}

void interface::btn_one_slot()
{
    ui->btn_all->setEnabled(true);
    ui->btn_one->setEnabled(false);
    this->ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
    ui->btn_send->setEnabled(true);

    chatType = 1;
    strcpy(msg.to_user_id  , to_user_id.toStdString().c_str());

    delete_Msg();
}

void interface::btn_all_slot()
{
    ui->btn_all->setEnabled(false);
    ui->chatuser->setText("everyone");
    ui->btn_one->setEnabled(true);
    ui->btn_send->setEnabled(true);
    chatType = 0;
    to_user_id = "";
     strcpy(msg.to_user_id,to_user_id.toStdString().c_str());
    delete_Msg();
}

void interface::btn_send_slot()
{
    QString msg_str = ui->lineEdit_msg->text();
    ui->lineEdit_msg->clear();
    if(msg_str == "")
        return;
    strcpy(msg.to_user_id , to_user_id.toStdString().c_str());
    strcpy(msg.msg,msg_str.toStdString().c_str());
    msg.service_type = 1;
    send(sockfd,&msg,sizeof(msg),0);
    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem(QString(msg.msg) + " :我"));
    QTableWidgetItem* item = ui->tableWidget_msgPage->item(row_count,0);
    item->setTextAlignment(Qt::AlignRight);
}

void interface::recv_msg(Msg* m)
{
    if(m->service_type == 1)
    {
        if(chatType == 1)//one mode
        {
            if(strcmp(m->to_user_id, "") == 0)
            {
                return;
            }
            to_user_id = QString(m->from_user_id);
            ui->btn_send->setEnabled(true);
        }
        if(0 == chatType) //everyone mode
        {
            if(strcmp(m->to_user_id, "") != 0)
                return;
        }

        if(strcmp(m->from_user_id, this->msg.from_user_id)==0 && m->service_type != 2)//all except myself
            return;
        int row_count = ui->tableWidget_msgPage->rowCount();
        ui->tableWidget_msgPage->insertRow(row_count);
        ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem(QString(m->from_user_id) +": " + QString(m->msg)));
    }
    else if(m->service_type == 3)
    {
        int row_count = ui->tableWidget_msgPage->rowCount();
        ui->tableWidget_msgPage->insertRow(row_count);
        ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem(QString(m->from_user_id) +": " + "send you a file"));
    }
}

void interface::recv_user(Msg*m, int i)
{
    ui->tableWidget_userlist->insertRow(i);
    ui->tableWidget_userlist->setItem(i,0,new QTableWidgetItem(QString(m->msg)));
    qDebug() << "userID: " << m->msg << endl;
}

void interface::delete_Msg()//clean windows
{
    int row_count = ui->tableWidget_msgPage->rowCount();
    while(row_count)
    {
        row_count--;
        ui->tableWidget_msgPage->removeRow(row_count);
    }
}

void interface::delete_Onlineuser()
{
    int row_count = ui->tableWidget_userlist->rowCount();
    while(row_count)
    {
        row_count--;
        ui->tableWidget_userlist->removeRow(row_count);
    }
}


void interface::btn_refresh_slot()//refresh button
{
    msg.service_type = 2;
    strcpy(msg.msg,"refresh");
    send(sockfd, &msg, sizeof(msg), 0);
    delete_Onlineuser();
}


void interface::btn_send_file_slot()// send file button
{
    QString fname = ui->lineEdit_msg->text();
    ui->lineEdit_msg->clear();
    if(fname== "")
        return;
    file *f = new file (fname.toStdString().c_str());
    if(f->open() == 0){
        return;
    }
    int length = f->file_length();
    char *p;
    p = (char *)(&length);
    strncpy(msg.msg, p,sizeof(int));
    p = msg.msg;
    p += sizeof(int);
    strcpy(msg.to_user_id , to_user_id.toStdString().c_str());
    strcpy(p,fname.toStdString().c_str());
    msg.service_type = 3;
    send(sockfd,&msg,sizeof(msg),0);
    usleep(1000);
    f->send_file(sockfd);
    f->close();
    delete f;


    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem("send a file: "+fname + " :我"));
   QTableWidgetItem* item = ui->tableWidget_msgPage->item(row_count,0);
    item->setTextAlignment(Qt::AlignRight);

}