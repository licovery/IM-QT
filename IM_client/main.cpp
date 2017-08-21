#include "login.h"
#include "connectserver.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
