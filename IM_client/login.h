#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "interface.h"
#include <QTcpSocket>


namespace Ui
{
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private:
    Ui::Login *ui;
    MainInterface *main_face;
    QTcpSocket *tcpSocket;
    QString clientName;

public slots:
    void loginTo();
    void loginCheck();
    void serverClose();
    //    void registerTo();
};

#endif // LOGIN_H
