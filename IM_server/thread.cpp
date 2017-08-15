#include "thread.h"

Thread::Thread(User user, UserList *list):
    my_user(user), online_list(list) {
}

Thread::~Thread() {

}

void Thread::start() {


    int ret = pthread_create(&thread, NULL, run, this);
    if(ret != 0)
        cout << "create thread is fail" << endl;
}

void Thread::send_online_user() {
    Msg msg;
    memset(msg.msg, 0, sizeof(msg.msg));
    msg.service_type = 2;

    int len = online_list->length();
    cout << "online list length: " << len << endl;
    strncpy(msg.msg, (char *)&len, sizeof(int));

    send(my_user.csockfd, &msg, sizeof(msg), 0);//send length to client

    vector<User> ulist;
    ulist = online_list->getOnlineUsers();

    for (auto it = ulist.begin(); it != ulist.end(); it++) {
        memset(msg.msg, 0, sizeof(msg.msg));
        msg.service_type = 2;
        strcpy(msg.msg, it->id.c_str());
        send(my_user.csockfd, &msg, sizeof(msg), 0);
        cout << "send " << msg.msg << " to " << my_user.id << " finish" << endl;
    }
    cout << "send online list to " << my_user.id << " finish" << endl;
}

void* Thread::run(void* arg) {
    cout << "new thread is started" << endl;
    Thread* th = (Thread*)(arg);

    //send online_user

    /*int length = th->online_list->length();
    cout << "length:" << length << endl;
    //send online_user length to client_user
    send(th->my_user.csockfd, &length, sizeof(int), 0);

    //send all the online userinfo to client_user
    vector<User> ulist = th->online_list->getOnlineUsers();

    for (auto it = ulist.begin(); it != ulist.end(); it++) {
        char id[20];
        memset(&id, 0, sizeof(id));
        strcpy(id, it->id.c_str());
        send(th->my_user.csockfd, &id, sizeof(id), 0);
    }
    */
    th->send_online_user();
    //cout << "first send online list to " << th->my_user.id << " finish" << endl;

    Msg msg;
    vector<User> ulist = th->online_list->getOnlineUsers();
    while(1) {
        int ret = recv(th->my_user.csockfd, &msg, sizeof(msg), 0);
        if(ret == -1 || ret == 0) {
            th->online_list->remove(th->my_user);
            cout << "user " << th->my_user.id << " " << " offline" << endl;
            close(th->my_user.csockfd);
            break;
        }

        if(msg.service_type == ONLINEUSER) {
            th->send_online_user();
        } else if(msg.service_type == CHAT) {
            //is all or is one ?
            cout << "recv a message from " << msg.from_user_id << ": " << msg.msg << endl;

            if(strcmp(msg.to_user_id, "") == 0) { //send to everyone

                for (auto it = ulist.begin(); it != ulist.end(); it++) {
                    if(strcmp(it->id.c_str() , msg.from_user_id) != 0) { //except sender
                        int ret = send(it->csockfd, &msg, sizeof(msg), 0);
                        if (ret == -1 || ret == 0) {
                            cout << "send message to " << it->id << " fail" << endl;
                        } else {
                            cout << "send message to " << it->id << " finish" << endl;
                        }

                    }
                }
            } else { //to someone

                int csockfd = th->online_list->selectSockfd(string (msg.to_user_id));
                int ret = send(csockfd, &msg, sizeof(msg), 0);

                if(ret == -1 || ret == 0) {
                    cout << "send message to " << msg.to_user_id << " fail" << endl;
                } else {
                    cout << "send message to " << msg.to_user_id << " finish" << endl;
                }

            }
        } else if(msg.service_type == SENDFILE) { // file mode
            int file_len, *p;
            char file_name[128], *c;
            p = (int *)(msg.msg);
            c = msg.msg + sizeof(int);
            file_len = *p;
            strcpy(file_name, c);
            file *f = new file(file_name);
            f->set_length(file_len);
            f->recv_file(th->my_user.csockfd);

            cout << "recv a file from " << msg.from_user_id <<  endl;

            if(strcmp(msg.to_user_id, "") == 0) { // all mode

                for (auto it = ulist.begin(); it != ulist.end(); it++) {
                    if(strcmp(it->id.c_str() , msg.from_user_id) != 0) { //except sender
                        int ret = send(it->csockfd, &msg, sizeof(msg), 0);
                        f->send_file(it->csockfd);
                        if(ret == -1 || ret == 0) {
                            cout << "send file to " << it->id << " fail" << endl;
                        } else {
                            cout << "send file to " << it->id << " finish" << endl;
                        }
                    }
                }
                f->fr();
            } else {

                int csockfd = th->online_list->selectSockfd(string (msg.to_user_id));
                int ret = send(csockfd, &msg, sizeof(msg), 0);
                f->send_file(csockfd);
                f->fr();
                cout << csockfd << endl;
                if(ret == -1 || ret == 0) {
                    cout << "send file to " << msg.to_user_id << " fail" << endl;
                } else {
                    cout << "send file to " << msg.to_user_id << " finish" << endl;
                }
            }
        }

    }
    cout << "thread " << th->thread << " is die" << endl;
    return NULL;
}
