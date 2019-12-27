#include "server.h"
#include <QTcpSocket>
#include <iostream>
#include <memory>
#include "msg.h"


Server::Server()
{
    list = new UserList();
    qServer = new QTcpServer;
    socketPool = new std::set<QTcpSocket *>;

    // 增加一个服务器状态字段
    if (!init())
    {
        qDebug() << "init error";
    }
    else
    {
        qDebug() << "init succ";
    }
}

Server::~Server()
{
    delete list;
    delete qServer;
    delete socketPool;
}

void Server::addUser(const std::string &id, QTcpSocket *tcpsocket)
{
    User *newUser = new User(id, tcpsocket);
    list->add(newUser);
}

void Server::removeUser(User *user)
{
    assert(user != nullptr);
    list->remove(user);
    socketPool->erase(user->getSocket());
    delete user;
}

LoginResponse::Status Server::checkUserLogin(const std::string &id) const
{
    if (list->in(id))
    {
        return LoginResponse::ID_CONFLICT;
    }
    else
    {
        return LoginResponse::SUCC;
    }
}

std::vector<User *> Server::onlineUsers() const
{
    return list->getOnlineUsers();
}

bool Server::sendMsg(const Transaction &transaction, QTcpSocket *sock)
{
    if (sock == nullptr)
    {
        auto dstUser = list->getUserInfo(transaction.getToUserId());
        assert(dstUser != nullptr);
        sock = dstUser->getSocket();
        assert(sock != nullptr);
    }

    if (!sock->isWritable())
    {
        std::cerr << transaction.getToUserId() << " socket can not write" << std::endl;
        return false;
    }

    std::cout << "====send====" << std::endl;
    transaction.debug();

    std::string output = transaction.getRsp();
    sock->write(output.c_str(), output.size());
    return true;
}

void Server::serverProc(const QByteArray &rcvTcpStream, QTcpSocket *readSocket)
{
    Transaction transaction;
    auto serviceType = transaction.parseReq(rcvTcpStream);

    // 上一个函数应该返回data,server处理data返回对应数据给transaction
    if (serviceType == Transaction::LOGIN)
    {
        auto status = checkUserLogin(transaction.getFromUserId());
        if (status == LoginResponse::SUCC)
        {
            addUser(transaction.getFromUserId(), readSocket);
            list->debug();
        }
        transaction.buildLoginRsp(status);
        sendMsg(transaction, readSocket);
    }
    else if (serviceType == Transaction::ONLINE_SEARCH)
    {
        transaction.buildOnlineUsersRsp(onlineUsers());
        sendMsg(transaction);
    }
    else if (serviceType == Transaction::CHAT_MSG)
    {
        transaction.buildChatMsgRsp();
        sendMsg(transaction);
    }
}

//初始化服务器到监听状态
bool Server::init()
{
    if (qServer == Q_NULLPTR)
    {
        qDebug() << "new error";
        return false;
    }
    if (!qServer->listen(QHostAddress::Any, 8888))
    {
        qDebug() << "listen error";
        qServer->close();
        return false;
    }
    //新连接到来的处理
    connect(qServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    return true;
}


void Server::acceptConnection()
{
    QTcpSocket *connSocket = qServer->nextPendingConnection();
    if (connSocket == nullptr)
    {
        return;
    }

    //首次连接,把网络连接加入连接池
    socketPool->insert(connSocket);

    // 设定该连接相关的信号处理函数
    // 客户端发送数据过来
    connect(connSocket, SIGNAL(readyRead()), this, SLOT(readMsg()));
    // 客户端连接关闭
    connect(connSocket, SIGNAL(disconnected()), this, SLOT(closeConnection()));

}

void Server::readMsg()
{
    // 遍历userlist,看看哪个user可读
    qDebug() << "readMsg";
    QTcpSocket *activeConn = dynamic_cast<QTcpSocket *>(sender());

    if (activeConn->isReadable())
    {
        serverProc(activeConn->readAll(), activeConn);
    }
}

void Server::closeConnection()
{
    QObject *closedSocket = sender();

    for (auto curUser : list->getOnlineUsers())
    {
        QTcpSocket *curSocket = curUser->getSocket();
        if (curSocket == closedSocket)
        {
            std::cout << curUser->getId() << " disconnect" << std::endl;
            // 关闭用户连接
            curSocket->close();
            // 从在线用户中移除
            list->remove(curUser->getId());
            // 删除该用户
            delete curUser;
            break;
        }
    }

    QTcpSocket *sock = dynamic_cast<QTcpSocket *>(closedSocket);
    sock->close();

    list->debug();
}
