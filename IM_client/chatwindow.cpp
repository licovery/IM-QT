#include "chatwindow.h"
#include "ui_interface.h"
#include "file.h"
#include "msg.h"
#include <QTime>
#include <iostream>
#include <QMessageBox>

ChatWindow::ChatWindow(QString peerId, QWidget *parent): QWidget(parent), ui(new Ui::ChatWindow), peerId(peerId)
{
    ui->setupUi(this);

    ui->info->setText("chat with " + peerId);



    ui->tableWidget_msgPage->setColumnCount(1);
    ui->tableWidget_msgPage->setShowGrid(false);
    ui->tableWidget_msgPage->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget_msgPage->verticalHeader()->setVisible(false);
    ui->tableWidget_msgPage->horizontalHeader()->setVisible(false);
    ui->tableWidget_msgPage->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // tableWidget_msgPage 每次更新自动聚焦到最新的item


//        connect(ui->tableWidget_userlist, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(getItem(QTableWidgetItem *)));

//        connect(ui->btn_one, SIGNAL(clicked()), this, SLOT(btn_one_slot()));
//        connect(ui->btn_all, SIGNAL(clicked()), this, SLOT(btn_all_slot()));
    connect(ui->btn_send, SIGNAL(clicked()), this, SLOT(btn_send_slot()));
//        connect(ui->btn_refresh, SIGNAL(clicked()), this, SLOT(btn_refresh_slot()));
//        connect(ui->btn_send_file, SIGNAL(clicked()), this, SLOT(btn_send_file_slot()));



}

ChatWindow::~ChatWindow()
{
    delete ui;
}



void ChatWindow::printRecvMsg(const std::string &content)
{
    auto line = QTime::currentTime().toString();
    line += " ";
    line += content.c_str();

    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count, 0, new QTableWidgetItem(line));
}


void ChatWindow::btn_send_slot()
{
    QString sendStr = ui->textEdit->toPlainText();
    QString msg_str = sendStr;
    ui->textEdit->clear();
    if (msg_str == "") return;

    msg_str = QTime::currentTime().toString() + " 我: " + msg_str;

    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count, 0, new QTableWidgetItem(msg_str));
    QTableWidgetItem *item = ui->tableWidget_msgPage->item(row_count, 0);
    item->setTextAlignment(Qt::AlignRight);

    emit sendMessage(peerId, sendStr);
}


#if 0
void MainInterface::getItem(QTableWidgetItem *item)
{
    // 用户点击了一下用户列表的某一项

    item->data(Qt::DisplayRole).toString();
    ui->chatuser->setText("You are talking to: " + to_user_id);
//    strcpy(msg.to_user_id, to_user_id.toStdString().c_str());
    ui->btn_send->setEnabled(true);

}


void interface::btn_one_slot()
{
    ui->btn_all->setEnabled(true);
    ui->btn_one->setEnabled(false);
    ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
    to_user_id = ui->tableWidget_userlist->item(0, 0)->data(Qt::DisplayRole).toString();
    ui->chatuser->setText("You are talking to: " + to_user_id);
    chatType = 1;
    strcpy(msg.to_user_id, to_user_id.toStdString().c_str());
    ui->tableWidget_msgPage->clearContents();
}

void interface::btn_all_slot()
{
    ui->chatuser->setText("You are talking to: everyone");
    ui->btn_all->setEnabled(false);
    ui->btn_one->setEnabled(true);
    ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::NoSelection);
    chatType = 0;
    to_user_id = "";
    strcpy(msg.to_user_id, to_user_id.toStdString().c_str());
    ui->tableWidget_msgPage->clearContents();
}





void interface::recv_user(Msg *m, int i)
{
    ui->tableWidget_userlist->insertRow(i);
    ui->tableWidget_userlist->setItem(i, 0, new QTableWidgetItem(QString(m->msg)));
    qDebug() << "userID: " << m->msg << endl;
}





void interface::btn_refresh_slot()//refresh button
{
    msg.service_type = 2;
    strcpy(msg.msg, "refresh");
    send(sockfd, &msg, sizeof(msg), 0);
    ui->tableWidget_userlist->clearContents();
}


void interface::btn_send_file_slot()// send file button
{
    QString fname = ui->lineEdit_msg->text();
    ui->lineEdit_msg->clear();
    if (fname == "")
        return;
    file *f = new file (fname.toStdString().c_str());
    if (f->open() == 0)
    {
        return;
    }
    int length = f->file_length();
    char *p;
    p = (char *)(&length);
    strncpy(msg.msg, p, sizeof(int));
    p = msg.msg;
    p += sizeof(int);
    strcpy(msg.to_user_id, to_user_id.toStdString().c_str());
    strcpy(p, fname.toStdString().c_str());
    msg.service_type = 3;
    send(sockfd, &msg, sizeof(msg), 0);
    usleep(1000);
    f->send_file(sockfd);
    f->close();
    delete f;


    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count, 0, new QTableWidgetItem("send a file: " + fname + " :我"));
    QTableWidgetItem *item = ui->tableWidget_msgPage->item(row_count, 0);
    item->setTextAlignment(Qt::AlignRight);

}
#endif
