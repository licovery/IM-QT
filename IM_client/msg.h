#ifndef MSG_H
#define MSG_H

#include "im.pb.h"
#include <string>
#include <QByteArray>
#include <QDebug>
#include <iostream>

struct MsgBuilder
{
    static std::string buildLoginReq(const std::string &clientId)
    {
        Interface pbMsg;

        auto loginReq = pbMsg.mutable_login_req();
        loginReq->set_user_id(clientId);
        std::cout << pbMsg.DebugString() << std::endl;

        return pbMsg.SerializeAsString();
    }

    static std::string buildOnlineUsersReq(const std::string &clientId)
    {
        Interface pbMsg;
        pbMsg.mutable_online_req()->set_from_user_id(clientId);

        std::cout << pbMsg.DebugString() << std::endl;
        return pbMsg.SerializeAsString();
    }

    static std::string buildChatMsg(const std::string &content, const std::string &srcId, const std::string &dstId)
    {
        Interface pbMsg;
        auto msg = pbMsg.mutable_msg();
        msg->set_service(InstantMessaging::CHAT);
        msg->set_from_user_id(srcId);
        msg->set_to_user_id(dstId);
        msg->set_chat_msg(content);

        std::cout << pbMsg.DebugString() << std::endl;
        return pbMsg.SerializeAsString();
    }
private:

};


struct MsgParser
{
    static LoginResponse::Status parseLoginRsp(const LoginResponse &loginRsp)
    {
        return loginRsp.result();
    }

    static std::vector<std::string> parseOnlineUsers(const OnlineUserResponse &onlineRsp)
    {
        std::vector<std::string> res;
        for (int i = 0; i < onlineRsp.online_user_size(); i++)
        {
            res.push_back(onlineRsp.online_user(i));
        }
        return res;
    }

    static std::string parseChatMsg(const InstantMessaging &imMsg)
    {
        std::string res;
        res += imMsg.from_user_id();
        res += ": ";
        res += imMsg.chat_msg();
        return res;
    }
};

#endif // MSG


