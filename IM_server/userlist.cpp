/*
用户链表，负责维护当前所有在线的用户，有新用户上线则添加到链表中，有用户下线则从该链表中删除
*/
#include "userlist.h"



UserList::UserList() {

}

UserList::~UserList() {

}

void UserList::add(User user) {
    mlist[user.id] = user.csockfd;
}

void UserList::remove(User user) {
    auto it = mlist.find(user.id);
    if (it != mlist.end())
        mlist.erase(it);
}

int UserList::length() {
    return mlist.size();
}

int UserList::selectSockfd(string id) {
    return mlist[id];
}

vector<User> UserList::getOnlineUsers() {
    vector<User> ulist;
    for (auto it = mlist.begin(); it != mlist.end(); it++) {
        ulist.push_back(User (it->first, it->second));
    }
    return ulist;
}


void UserList::show() {
    cout << "show online userlist" << endl;

    for (auto it = mlist.begin(); it != mlist.end(); it++) {
        cout << "userID: " << it->first << "  userSockfd: " << it->second << endl;
    }
}
