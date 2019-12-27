#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <iostream>
#include <QCloseEvent>
#include <QTableWidgetItem>


namespace Ui
{
class MainInterface;
}




class MainInterface : public QWidget
{
    Q_OBJECT
public:
    explicit MainInterface(QTcpSocket *sockfd, QString my_id, QWidget *parent = 0);
    ~MainInterface();
protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainInterface *ui;
    QTcpSocket *tcpSocket;

    int chatType;//0 is all ,1 is one

    QString my_id;
    QString to_user_id;

private slots:
    void getItem(QTableWidgetItem *item);

//    void recv_msg(Msg *m);
//    void recv_user(Msg *m, int i);

//    void btn_one_slot();
//    void btn_all_slot();
//    void btn_send_slot();
//    void btn_send_file_slot();
//    void btn_refresh_slot();
};



#endif // INTERFACE_H
