#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <iostream>

MainWindow::MainWindow(QString myId, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), myId(myId)
{
    ui->setupUi(this);

    ui->myid->setText(myId);

    ui->tableWidget_userlist->setColumnCount(1);//设置column为1
    ui->tableWidget_userlist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList list;
    list << "userID";
    ui->tableWidget_userlist->setHorizontalHeaderLabels(list);
    ui->tableWidget_userlist->verticalHeader()->setVisible(false); //隐藏行表头
    ui->tableWidget_userlist->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
//    ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_userlist->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改

    // 不能选择自己
    connect(ui->tableWidget_userlist, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(openChatWindow(QTableWidgetItem *)));
}



void MainWindow::printOnlineUserInfo(const std::vector<std::string> &users)
{
    //刷新列表，删除原来的内容
    ui->tableWidget_userlist->clearContents();
    ui->tableWidget_userlist->setRowCount(users.size());
    for (int i = 0; i < users.size(); i++)
    {
        ui->tableWidget_userlist->setItem(i, 0, new QTableWidgetItem(QString(users[i].c_str())));
        std::cout << "userID: " << users[i] << std::endl;
    }
}

void MainWindow::printRecvMsg(const std::string &peerId, const std::string &msg)
{
    QString id(peerId.c_str());
    openChatWindow(id);
    chatUiPool[id]->printRecvMsg(msg);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for (auto it = chatUiPool.begin(); it != chatUiPool.end(); it++)
    {
        closeChatWinow(it->second);
    }
    chatUiPool.clear();
}

void MainWindow::openChatWindow(QTableWidgetItem *item)
{
    openChatWindow(item->text());
}

void MainWindow::openChatWindow(const QString &peerId)
{
    // 不存在则创建
    if (chatUiPool.count(peerId) == 0)
    {
        auto chatUi = new ChatWindow(peerId);
        connect(chatUi, SIGNAL(sendMessage(QString, QString)), this, SLOT(signalTrans(QString, QString)));
        chatUiPool[peerId] = chatUi;
        chatUi->show();
    }
    else
    {
        auto chatWindow = chatUiPool[peerId];
        chatWindow->show();
    }
}

// 只有主界面关闭才释放内存
void MainWindow::closeChatWinow(ChatWindow *w)
{
    if (w != nullptr)
    {
        w->close();
        delete w;
    }
}

void MainWindow::signalTrans(QString dstId, QString content)
{
    emit sendMessage(dstId, content);
}



MainWindow::~MainWindow()
{
    delete ui;
}
