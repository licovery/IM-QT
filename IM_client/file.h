#if 0

#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <QDebug>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class file
{
public:
    file(const char *name);
    void recv_file(int fd, int length);
    void send_file(int fd);
    int file_length();
    int open();
    void close();
private:
    FILE *fp;
    char filename[128];
};

#endif // FILE_H
#endif
