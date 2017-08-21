#ifndef USER
#define USER

#include <arpa/inet.h>
#include <string>

#define LOGIN 1
#define REGISTER 2

using namespace std;

struct userInfo {
    char id_name[20] = "";
    char pwd[20] = "";
    int mode;//login or register
};

class User {
public:
    User(string id, int csockfd);
    User();
    string id;
    int csockfd;
};

#endif // USER
