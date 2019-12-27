#ifndef USER_H
#define USER_H


#include <string>
#include <QTcpSocket>


class User
{
public:

    User(std::string id, QTcpSocket *qSocket);
    std::string getId() const;
    QTcpSocket *getSocket() const;

private:
    std::string userId;
    QTcpSocket *userSocket;

};

#endif // USER
