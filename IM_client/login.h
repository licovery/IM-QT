#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "userinfo.h"
#include "connectserver.h"
#include "interface.h"


namespace Ui {
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
    interface* main_face;
    int sockfd;
    connectServer cs;

private slots:
    void loginTo();
    void registerTo();
};

#endif // LOGIN_H
