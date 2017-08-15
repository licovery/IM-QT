#include "user.h"

User::User(string id, int csockfd):
    id(id), csockfd(csockfd) {

}

User::User() {}
