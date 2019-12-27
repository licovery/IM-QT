#ifndef USERLIST_H
#define USERLIST_H

#include <string>
#include <set>
#include <vector>
#include "user.h"


class UserList
{
public:
    void add(User *user);
    void remove(User *user);
    void remove(const std::string &id);
    User *getUserInfo(const std::string &id);
    bool in(const std::string &id);
    std::vector<User *> getOnlineUsers();
    void debug();
private:
    std::set<User *> users;
};

#endif // USERLIST_H
