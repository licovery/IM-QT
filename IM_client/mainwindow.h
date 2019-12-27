#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include "chatwindow.h"
#include <QTableWidget>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString myId, QWidget *parent = nullptr);
    ~MainWindow() override;

    void printOnlineUserInfo(const std::vector<std::string> &users);
    void printRecvMsg(const std::string &peerId, const std::string &msg);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    std::map<QString, ChatWindow *> chatUiPool;
    QString myId;
public slots:
    void openChatWindow(QTableWidgetItem *item);
    void openChatWindow(const QString &peerId);
    void closeChatWinow(ChatWindow *w);
    void signalTrans(QString dstId, QString content);
signals:
    void sendMessage(QString, QString);
};

#endif // MAINWINDOW_H
