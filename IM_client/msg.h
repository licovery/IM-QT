#ifndef MSG
#define MSG

struct Msg
{
    int service_type;
    char to_user_id[20];//if user_id is 0,perform all user
    char  from_user_id[20];
    char msg[256];//msg text
};
#endif // MSG

//type   1 chat
//          2 online_user
//          3 file
