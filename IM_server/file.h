#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class file
{
public:
    file(char *name);
    ~file();
    void send_file(int fd);
    void recv_file(int fd);
    void set_length(int i);
    void fr();

private:
    int length;
    char filename[128];
    FILE *fp;
    char *buffer;
};


#endif // FILE_H_INCLUDED
