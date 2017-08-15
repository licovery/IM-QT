#include "recvmsgthread.h"
#include <unistd.h>
#include "file.h"

recvMsgThread::recvMsgThread(int sockfd, QObject *parent):
    sockfd(sockfd),QObject(parent)
{

}

recvMsgThread::~recvMsgThread()
{

}

void recvMsgThread::start()
{
    int ret = pthread_create(&thread,NULL,run,this);
    if(0!= ret)
        qDebug() << "create thread is fail" << endl;
    else{
        qDebug() << "create thread is successful" << endl;
    }
}

void* recvMsgThread::run(void* arg)
{
    recvMsgThread* th = (recvMsgThread*)(arg);
    Msg msg;
    //for mode 2
    while(1)
    {
//        usleep(1000);
        //memset(&msg,0,sizeof(msg));
        int ret = recv(th->sockfd,&msg,sizeof(msg),0);
        if(-1 == ret || 0 == ret)
            break;
        if(msg.service_type == 1)//chat mode
        {
            emit th->sendMsg_signal(&msg);
            qDebug() << "recv a message from " << msg.from_user_id << ": " << msg.msg << endl;
        }
        else if(msg.service_type == 2)//online user update mode
        {
            int length = 0;
            length = *((int*)(msg.msg));
            qDebug() << "online user count is:" << length << endl;
            for (int i = 0; i < length; i++) {
                recv(th->sockfd,&msg,sizeof(msg),0);
                emit th->refresh_signal(&msg, i);
            }

            /*
            len = (int*)(msg.msg);
            length  = *len;
            while(length--)
            {
                recv(th->sockfd,&msg,sizeof(msg),0);
                emit th->refresh_signal(&msg, i);
            }*/
        }

        else if(msg.service_type == 3)//file transfer mode
        {
            emit th->sendMsg_signal(&msg);
            int file_len, *p;
            char file_name[128], *c;
            p = (int *)(msg.msg);
            c = msg.msg + sizeof(int);
            file_len = *p;
            strcpy(file_name, c);
            file *f = new file(file_name);
            f->recv_file(th->sockfd, file_len);
        }
    }
}
