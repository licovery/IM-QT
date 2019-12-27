#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>


namespace Ui
{
class ChatWindow;
}



class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QString peerId, QWidget *parent = nullptr);
    ~ChatWindow();

    void printRecvMsg(const std::string &content);

private:
    Ui::ChatWindow *ui;

    QString peerId;


private slots:
//    void getItem(QTableWidgetItem *item);

//    void recvMsg();
//    void recv_user(Msg *m, int i);

//    void btn_one_slot();
//    void btn_all_slot();
    void btn_send_slot();
//    void btn_send_file_slot();
//    void btn_refresh_slot();
signals:
    void sendMessage(QString, QString);
};



#endif // CHATWINDOW_H
