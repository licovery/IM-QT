#ifndef MSG_H
#define MSG_H

#include "im.pb.h"
#include <QByteArray>
#include <string>
#include <vector>
#include "user.h"


class Transaction
{
public:
    enum ServiceType
    {
        LOGIN,
        ONLINE_SEARCH,
        CHAT_MSG,
    };

    Transaction()
    {

    }

    ServiceType parseReq(const QByteArray &tcpstream);
    ServiceType serviceType() const;

    void buildLoginRsp(LoginResponse::Status status);
    void buildOnlineUsersRsp(std::vector<User *> onlineUsers);
    void buildChatMsgRsp();

    std::string getRsp() const;
    std::string getFromUserId() const;
    std::string getToUserId() const;
    void debug() const;

private:
    std::string fromUserId;
    std::string toUserId;
    ServiceType service_type;
    Interface pbMsgReq;
    Interface pbMsgRsp;
};



// 单例
//class MsgProc
//{
//public:
//    static MsgProc *getInst()
//    {
//        static MsgProc singleInst;
//        return &singleInst;
//    }
//private:
//    MsgProc() {}
//    MsgProc(const MsgProc &);
//    MsgProc &operator=(const MsgProc &);
//};





#endif // MSG

