#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <iostream>
#include <QCloseEvent>
#include <QTableWidgetItem>
#include "connectserver.h"
#include "msg.h"
#include "recvmsgthread.h"

#define ALL 0
#define ONE 1


struct online_userinfo{
    char id[20];
};

namespace Ui {
class interface;
}

class interface : public QWidget
{
    Q_OBJECT

public:
    explicit interface(int sockfd,QString my_id,QWidget *parent = 0);
    ~interface();

protected:
    void closeEvent(QCloseEvent * event);

private:
    Ui::interface *ui;
    int sockfd;
    Msg msg;

    int chatType;//0 is all ,1 is one

    QString my_id;
    QString to_user_id;

    recvMsgThread* thread;
    //void recv_Online_userinfo();


private slots:
    void getItem(QTableWidgetItem* item);

    void recv_msg(Msg *m);
    void recv_user(Msg*m, int i);

    void btn_one_slot();
    void btn_all_slot();
    void btn_send_slot();
    void btn_send_file_slot();
    void btn_refresh_slot();
};

#endif // INTERFACE_H
