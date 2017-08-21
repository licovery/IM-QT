#include <iostream>
#include <string>
#include <stdlib.h>
#include "server.h"

using namespace std;

int main() {
    Server server;
    while(1) {
        bool status = server.acceptClient();
        if(status == false) {
            server.closeServer();
            cout << "server error\n";
            exit(-1);
        }
    }
    return 0;
}

