#include "file.h"

file::file(const char* name)
{
    strcpy(filename, name);
    fp = NULL;
}

int file::file_length()
{
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind (fp);
    return size;
}

void file::recv_file(int fd, int length)
{
    char *buffer, *p;
    int n, mod;
    int result;
    n = length / 1024;
    mod = length % 1024;
    buffer = (char *)malloc(sizeof(char)*length);
    if(buffer == NULL){
        qDebug() << "malloc error" << endl;
        return;
    }
    p = buffer;
    while(n--)
    {
        recv(fd, p, sizeof(char)*1024, 0);
        p+=1024;
    }
    if(mod)
    {
        recv(fd, p, sizeof(char)*mod, 0);
    }


    fp = fopen(filename, "w");
    if(fp == NULL){
        qDebug() << "fopen error" << endl;
    }
    result = fwrite(buffer, sizeof(char), length, fp);
    if(result != length){
        qDebug() << "fwrite error";
    }
    free(buffer);
    fclose(fp);
    fp = NULL;
}

int file::open()
{
    fp = fopen(filename, "r");
    if(fp == NULL){
        qDebug() << "fopen error" << endl;
        return 0;
    }
    return 1;
}

void file::close(){
    fclose(fp);
}

void file::send_file(int fd)
{
   int length, result;
   char * buffer, *p;
    length = file_length();
    int n = length / 1024;
    int mod = length % 1024;
    buffer = (char *)malloc(sizeof(char)*length);
    if(buffer == NULL){
        qDebug() << "malloc error" << endl;
        return;
    }
    result = fread(buffer, 1, length, fp);
    if( result != length){
            qDebug() << "fread error" << endl;
            return;
    }
    p = buffer;
    for(int i = 0; i< n; i++)
    {
        send(fd, p, sizeof(char)*1024, 0);
        p += 1024;
        //usleep(1000);
    }
    if(mod != 0)
    {
        send(fd, p, sizeof(char)*mod,0);
    }

    free(buffer);
}
