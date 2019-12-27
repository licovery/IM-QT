#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <set>
#include <vector>
#include "userlist.h"
#include "msg.h"



//class Server: public QObject
//{
//    Q_OBJECT
//public:
//    Server();
//    virtual ~Server();

//private slots:
//    bool acceptClient();
//    bool messageProc();
//    void closeClient();

//private:
//    QTcpServer *qServer;
//    UserList *list;
//    bool init();
//};


class Server: public QObject
{
    Q_OBJECT
public:
    Server();
    virtual ~Server();

    void addUser(const std::string &id, QTcpSocket *tcpsocket);
    void removeUser(User *user);

    LoginResponse::Status checkUserLogin(const std::string &id) const;
    std::vector<User *> onlineUsers() const;

    void serverProc(const QByteArray &rcvTcpStream, QTcpSocket *readSocket);

    bool sendMsg(const Transaction &transaction);

private slots:
    void acceptConnection();
    void readMsg();
    void closeConnection();

private:
    bool init();

    QTcpServer *qServer;
    UserList *list;
    std::set<QTcpSocket *> *socketPool;
};





#endif // SERVER_H
