#include "user.h"

User::User(std::string id, QTcpSocket *qSocket): userId(id), userSocket(qSocket)
{

}


std::string User::getId() const
{
    return userId;
}

QTcpSocket *User::getSocket() const
{
    return userSocket;
}




