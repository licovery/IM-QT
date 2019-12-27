#ifndef MSG_H
#define MSG_H

#include "im.pb.h"
#include <string>
#include <QByteArray>
#include <QDebug>
#include <iostream>

struct MsgBuilder
{
    static std::string buildLoginReq(std::string clientId)
    {
        Interface pbMsg;

        auto loginReq = pbMsg.mutable_login_req();
        loginReq->set_user_id(clientId);
        std::cout << pbMsg.DebugString() << std::endl;

        std::string output;
        pbMsg.SerializeToString(&output);

        return output;
    }
private:

};
#endif // MSG

//type   1 chat
//          2 online_user
//          3 file
