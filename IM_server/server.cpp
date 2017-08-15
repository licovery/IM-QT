#include "server.h"

Server::Server() {
    if(!init())
        cout << "-------------------------------\n"
                "init error\n"
                "-------------------------------\n";
    list = new UserList();
    db = new DBMysql("127.0.0.1", "root", "mysql123", "IM");
    db->db_connect();
}

Server::~Server() {
    delete list;
    delete db;
}

//初始化服务器到监听状态
bool Server::init() {
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&addr.sin_zero, 8);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //socket()调用失败
    if(sockfd == -1) {
        cout << "socket error" << endl;
        return false;
    }

    int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1) {
        close(sockfd);
        sockfd = -1;
        cout << "bind error" << endl;
        return false;
    }

    //queue-length = 100
    ret = listen(sockfd, 100);
    if(ret == -1) {
        close(sockfd);
        sockfd = -1;
        cout << "listen error" << endl;
        return false;
    }
    return true;
}

//接受客户端的请求，如果一个用户的用户名和密码都正确才算是连接上服务器。否则删除已连接套接字描述符
bool Server::acceptClient() {
    if(sockfd == -1)
        return false;

    struct sockaddr_in caddr;
    socklen_t caddrsize = sizeof(caddr);
    int csockfd = accept(sockfd, (struct sockaddr*)&caddr, &caddrsize);
    if(csockfd == -1) {
        return false;
    }

    userInfo userdata;

    //接收客户端登入请求
    int ret = recv(csockfd, (void*)&userdata, sizeof(userdata), 0);
    if(ret == -1 || ret == 0) {
        close(csockfd);
        return false;
    }

    int login_flag = 0;
    //登录模式
    if (userdata.mode == LOGIN) { //login
        string sqlstr("select * from user where id = '");
        sqlstr.append(userdata.id_name).append("'");
        char** row = db->db_select(sqlstr.c_str());
        if(row != NULL && !strcmp(userdata.id_name, row[0]) && !strcmp(userdata.pwd, row[1])) {

            //给客户端发送登录状态，登录成功发送1
            cout << userdata.id_name << " login successful" << endl;
            login_flag = 1;

            //将已经登录的用户保存到online用户列表中
            User user(userdata.id_name, csockfd);
            list->add(user);
            list->show();

            //start a thread  for current user
            send(csockfd, &login_flag, sizeof(int), 0);
            Thread* thread = new Thread(user, list);
            thread->start();
            cout << "user: " << userdata.id_name << " online" << endl;

        } else {
            login_flag = 0;
            send(csockfd, &login_flag, sizeof(int), 0);
            close(csockfd);
        }

    } else if (userdata.mode == REGISTER) { //register
        cout << "user name: " << userdata.id_name << endl << "pwd: " << userdata.pwd << endl << "register" << endl;
        string sqlstr("insert into user values('");
        sqlstr.append(userdata.id_name).append("','").append(userdata.pwd).append("')");
        db->db_insert(sqlstr.c_str());

        // send something to tell client register succeed
        char symbol[6];
        strcpy(symbol, "neoli");
        send(csockfd, symbol, sizeof(symbol), 0);
    }

    return true;
}

void Server::closeServer() {
    close(sockfd);
}
