#include "client.h"
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client client;
    client.connectToServer();
    client.startLogin();

    // 重试对话框
    return a.exec();
}
