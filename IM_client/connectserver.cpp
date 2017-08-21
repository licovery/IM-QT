#include "connectserver.h"

connectServer::connectServer()
{

}

connectServer::~connectServer()
{

}

int connectServer::connectTo(string ip_addr, short port)
{
    //init the socket addr
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    bzero(addr.sin_zero, sizeof(addr.sin_zero));

    //create sockfd
    int ssockfd = socket(AF_INET,SOCK_STREAM,0);
    if(ssockfd == -1)
    {
       qDebug() << "create socket error" << endl;
        return -1;
    }
    //connect to server
    int res = connect(ssockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(res == -1)
    {
        QMessageBox::warning(NULL, "Network Error", "connnect to server fail");
        close(ssockfd);
        qDebug() << "connect error" << endl;
        return -1;
    }
    qDebug() << "connect successful!" << endl;
    return ssockfd;
}


void connectServer::closeSockfd(int sockfd)
{
    close(sockfd);
}
