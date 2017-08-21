#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <string>

#include "user.h"
#include "userlist.h"
#include "msg.h"
#include "file.h"

struct online_userinfo{
    char id[20];
};

class Thread
{
public:
    Thread(User user,UserList* list);
    ~Thread();
    void send_online_user();
    void start();

private:
    pthread_t thread;
    User my_user;
    UserList* online_list;

    static void* run(void* arg);
};

#endif // THREAD_H
