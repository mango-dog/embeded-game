#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include "serialthread.h"

extern int fd;
extern char readbuf[10];

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void handleConnected();
    void handleDisconnected();
    void handleReadyRead();
    void handleError(QAbstractSocket::SocketError error);
    void openserial();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_tcp;
    Thread_one *thread_1;

    bool m_isSendingImage = false;
    qint64 m_imageSize = 0;
    qint64 m_bytesSent = 0;
    QByteArray m_imageData;
};
#endif // MAINWINDOW_H
