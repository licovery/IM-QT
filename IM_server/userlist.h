#ifndef USERLIST_H
#define USERLIST_H

#include <string>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include "user.h"
using namespace std;



/*struct Node
{
    User user;
    Node* next;
    Node(User user): user(user), next(NULL) {}
    Node() {}
};*/

class UserList
{
public:
    UserList();
    ~UserList();

    void add(User user);
    void remove(User user);
    int length();
    int selectSockfd(string id);
    void show();
    vector<User> getOnlineUsers();

private:
    map<string, int> mlist;
};

#endif // USERLIST_H
