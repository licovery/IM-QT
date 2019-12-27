#include "msg.h"
#include <iostream>

Transaction::ServiceType Transaction::parseReq(const QByteArray &tcpstream)
{
    // pb解码
    pbMsgReq.ParseFromArray(tcpstream.data(), tcpstream.size());
    //        std::cout << "====recv====\n" << pbMsgReq.DebugString() << std::endl;

    if (pbMsgReq.interface_case() == Interface::kLoginReq)
    {
        service_type = LOGIN;
        fromUserId = pbMsgReq.login_req().user_id();
    }
    else if (pbMsgReq.interface_case() == Interface::kOnlineReq)
    {
        service_type = ONLINE_SEARCH;
        fromUserId = pbMsgReq.online_req().from_user_id();

    }
    else if (pbMsgReq.interface_case() == Interface::kMsg)
    {
        service_type = CHAT_MSG;
        fromUserId = pbMsgReq.msg().from_user_id();
    }
    return service_type;
}

Transaction::ServiceType Transaction::serviceType() const
{
    return service_type;
}

void Transaction::buildLoginRsp(LoginResponse::Status status)
{
    toUserId = fromUserId;
    pbMsgRsp.mutable_login_rsp()->set_result(status);
}

void Transaction::buildOnlineUsersRsp(std::vector<User *> onlineUsers)
{
    toUserId = fromUserId;
    for (auto user : onlineUsers)
    {
        pbMsgRsp.mutable_online_rsp()->add_online_user(user->getId());
    }
}

void Transaction::buildChatMsgRsp()
{
    toUserId = fromUserId;
    pbMsgRsp = pbMsgReq;
}

std::string Transaction::getRsp() const
{
    return pbMsgRsp.SerializeAsString();
}

std::string Transaction::getFromUserId() const
{
    return fromUserId;
}

std::string Transaction::getToUserId() const
{
    return toUserId;
}

void Transaction::debug() const
{
    std::cout << pbMsgReq.DebugString() << std::endl;
    std::cout << pbMsgRsp.DebugString() << std::endl;
}

