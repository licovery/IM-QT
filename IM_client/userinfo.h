#ifndef USERINFO_H
#define USERINFO_H

#define LOGIN 1
#define REGISTER 2

struct UserInfo
{
  char id[20]={0};
  char pwd[20]={0};
  int mode;
};

#endif // USERINFO_H

