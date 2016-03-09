#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    totalSize=0;
    fileNameSize=0;
    reciveSize=0;
    fileName="";
    writeFile=NULL;
    block.resize(0);
    ui->progressBar->hide();
    ui->hostLineEdit->setText("127.0.0.1");
    ui->portLineEdit->setText("10801");
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &readMessage);//有数据到来时发出readyRead()，执行readMessage()
    connect(tcpSocket, static_cast<void (QTcpSocket:: *)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, &displayError);//出现错误时发出error()，执行displayError()函数
    connect(ui->pushButton_Link, &QPushButton::clicked, this, &newConnect);
    connect(tcpSocket,&QTcpSocket::disconnected,this,&finish);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnect()
{
    block.resize(0);
    tcpSocket->abort(); //取消已有的连接
    tcpSocket->connectToHost(ui->hostLineEdit->text(),ui->portLineEdit->text().toInt());
     //连接到主机，这里从界面获取主机地址和端口号
}

void MainWindow::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_5);
    ui->label_Message->setText("File receiving...");
    if( reciveSize <= (int)sizeof(quint64)*2 )
    {
        //如果有则保存到blockSize变量中，没有则返回，继续接收数据
        if( tcpSocket->bytesAvailable() >= (int)sizeof(quint64)*2 && fileNameSize==0 )
        {
            in>>totalSize>>fileNameSize;
            reciveSize+=(int)sizeof(quint64)*2;
            qDebug()<<"Receive file size:"<<totalSize;
            ui->progressBar->show();

        }
        if(tcpSocket->bytesAvailable() >=fileNameSize && fileNameSize!=0)
        {
            in>>fileName;
            reciveSize+=fileNameSize;
            writeFile= new QFile(fileName);
            if(!writeFile->open(QFile::WriteOnly))
            {
                qDebug() << "open file error!";
                return;
            }
            qDebug()<<"Create file:"<<fileName;
            ui->progressBar->setMaximum(totalSize);
            ui->progressBar->setMinimum(0);
            ui->progressBar->setValue(reciveSize);
            block.resize(0);
        }
        else
            return;
    }
    if( reciveSize < totalSize )
    {
        block=tcpSocket->readAll();
        reciveSize+=block.size();
        writeFile->write(block);
        block.resize(0);
    }
    if( reciveSize == totalSize )
    {
        writeFile->close();
        message="Receive file successed!";
        ui->label_Message->setText(message);
    }
    ui->progressBar->setValue(reciveSize);
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Receive Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        ui->label_Message->setText("Server connect error:Host Not Found.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Receive Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        ui->label_Message->setText("Server connect error:Connection Refused.");
        break;
    default:
        QMessageBox::information(this, tr("Receive Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
        ui->label_Message->setText(tr("Server connect error:%1.").arg(tcpSocket->errorString()));
    }
    ui->pushButton_Link->setDisabled(false);
}

void MainWindow::on_openFileButton_clicked()
{
    QDesktopServices::openUrl(QUrl(QDir::currentPath(), QUrl::TolerantMode));
}

void MainWindow::finish()
{
    totalSize=0;
    fileNameSize=0;
    reciveSize=0;
    fileName="";
    writeFile->close();
    block.resize(0);
    ui->pushButton_Link->setDisabled(false);
    ui->label_Message->setText("Client receive succesed,you can continue now.");
}

void MainWindow::on_pushButton_Link_clicked()
{
    ui->pushButton_Link->setDisabled(true);
    ui->label_Message->setText("Trying connect to server...");
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
