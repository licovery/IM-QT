#include "file.h"


file::file(char *name) {
    strcpy(filename, name);
    fp = NULL;
    buffer = NULL;
}

file::~file() {
    if(buffer != NULL) {
        free(buffer);
    }
}

void file::set_length(int i) {
    length = i;
}

void file::send_file(int fd) {
    int n, mod, se;
    char *p;
    p  = buffer;
    n = length / 1024;
    mod = length % 1024;
    while(n--) {
        //usleep(1000);
        se = send(fd, p, sizeof(char) * 1024, 0);
        p += 1024;
    }
    if(mod)
        se = send(fd, p, sizeof(char) * mod, 0);

    if(se == -1) {
        cout << "send file error" << endl;
        return;
    } else {
        cout << "send file successful" << endl;
    }
}

void file::fr() {
    free(buffer);
    buffer = NULL;
}


void file::recv_file(int fd) {
    int re;
    int n, mod;
    char *p;
    buffer = (char *)malloc(sizeof(char) * length);
    p = buffer;
    n = length / 1024;
    mod = length % 1024;
    for(int i = 0; i < n; i++) {
        re = recv(fd, p, 1024 * sizeof(char), 0);
        p += 1024;
    }
    if(mod) {
        re = recv(fd, p, mod * sizeof(char), 0);
    }

    if(re == -1) {
        cout << "revc file error" << endl;
        return;
    } else {
        cout << "revc file successful" << endl;
    }
}





