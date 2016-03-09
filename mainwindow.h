#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();

    void on_quitButton_clicked();

    void on_connectButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpSocket;
    QString message; //存放从服务器接收到的字符串
    qint64 totalSize; //传输文件总大小
    qint64 fileNameSize;//文件名大小
    qint64 reciveSize;//接受的文件大小
    qint64 CacheSize;//缓存的文件大小
    QString fileName; //文件名
    QFile *writeFile;
    QByteArray block;

//private slots:
    void newConnect();   //连接服务器
    void readMessage();  //接收数据
    void finish();
    void displayError(QAbstractSocket::SocketError socketError); //显示错误
};

#endif // MAINWINDOW_H
