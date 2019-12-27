#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui
{
class Login;
}

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    virtual ~Login();

private:
    Ui::Login *ui;

public slots:
    void loginTo();
//    void registerTo();
signals:
    void userLogin(QString);
};

#endif // LOGIN_H
