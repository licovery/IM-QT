#ifndef MSG
#define MSG

#define CHAT 1
#define ONLINEUSER 2
#define SENDFILE 3

struct Msg
{
    int service_type;
    char to_user_id[20];//if user_id is 0,perform all user
    char from_user_id[20];
    char msg[256];//msg text
};

#endif // MSG

