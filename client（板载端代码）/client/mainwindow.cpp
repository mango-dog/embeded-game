#include "mainwindow.h"
#include "agreement.h"
#include "ui_mainwindow.h"

int fd;
char readbuf[10];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_tcp(new QTcpSocket(this))
{
    ui->setupUi(this);
    m_tcp=new QTcpSocket(this);
    thread_1 = new Thread_one ();

    ui->lineEdit->setText("8899");
    ui->lineEdit_2->setText("192.168.3.86");
        //setWindowTitle("¿Í»§¶Ë");
    ui->pushButton_2->setEnabled(false);

        // ×´Ì¬À¸
        //m_statusLabel = new QLabel;
        //m_statusLabel->setPixmap(QPixmap(":/disconnected.png").scaled(20,20));
        //ui->statusBar->addWidget(new QLabel("×´Ì¬:"));
        //ui->statusBar->addWidget(m_statusLabel);

        // Á¬½ÓÐÅºÅ²Û
    connect(m_tcp, &QTcpSocket::connected, this, &MainWindow::handleConnected);
    connect(m_tcp, &QTcpSocket::disconnected, this, &MainWindow::handleDisconnected);
    connect(m_tcp, &QTcpSocket::readyRead, this, &MainWindow::handleReadyRead);
    connect(m_tcp, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
                this, &MainWindow::handleError);
    connect(thread_1,&Thread_one::revice, this ,&MainWindow::on_pushButton_3_clicked);
    on_pushButton_clicked();
    openserial();
    thread_1->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openserial()
{
    SerialPort c;
    fd = c.OpenSerialPort("/dev/ttyCH9344USB7",9600);
    c.SerialPortFlush(fd);
}

void MainWindow::on_pushButton_clicked()
{
    QString ip = ui->lineEdit_2->text();
    //qDebug()<<ip;
    quint16 port = ui->lineEdit->text().toUShort();
    //qDebug()<<port;
    m_tcp->connectToHost(ip, port);
}


void MainWindow::on_pushButton_2_clicked()
{
    m_tcp->disconnectFromHost();
}


void MainWindow::on_pushButton_3_clicked()
{
    QByteArray data(reinterpret_cast<const char*>(readbuf), n_read);
    QString hexStr = data.toHex(' ');
    m_tcp->write(hexStr.toUtf8());
}

void MainWindow::handleConnected()
{
    ui->textEdit->append("1");
    //m_statusLabel->setPixmap(QPixmap(":/connected.png").scaled(20,20));
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::handleDisconnected()
{
    ui->textEdit->append("2");
    //m_statusLabel->setPixmap(QPixmap(":/disconnected.png").scaled(20,20));
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::handleReadyRead()
{
    QByteArray data = m_tcp->readAll();
    ui->textEdit->append("ÊÕµ½Êý¾Ý: " + QString::fromUtf8(data));
}

void MainWindow::handleError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    ui->textEdit->append("´íÎó: " + m_tcp->errorString());
}
