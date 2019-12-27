/*
用户链表，负责维护当前所有在线的用户，有新用户上线则添加到链表中，有用户下线则从该链表中删除
*/
#include "userlist.h"
#include <QDebug>
#include <iostream>

void UserList::add(User *user)
{
    if (user == nullptr)
    {
        return;
    }
    users.insert(user);
}

void UserList::remove(User *user)
{
    users.erase(user);
}

void UserList::remove(const std::string &id)
{
    for (auto it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getId() == id)
        {
            users.erase(it);
            break;
        }
    }
}


User *UserList::getUserInfo(const std::string &id)
{
    for (auto it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getId() == id)
        {
            return *it;
        }
    }

    std::cerr << "no such user" << std::endl;
    return nullptr;
}



void UserList::debug()
{
    std::cout << "show online userlist, total: " << users.size() << std::endl;

    for (auto it = users.begin(); it != users.end(); it++)
    {
        std::cout << "userID: " << (*it)->getId() << " user: " << (*it) << std::endl;
    }
}

bool UserList::in(const std::string &id)
{
    return getUserInfo(id) != nullptr;
}

std::vector<User *> UserList::getOnlineUsers()
{
    std::vector<User *> res;
    for (auto it = users.begin(); it != users.end(); it++)
    {
        res.push_back(*it);
    }
    return res;
}
