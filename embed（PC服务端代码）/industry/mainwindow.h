#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sqldatabase.h"
#include "registersql.h"
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QBuffer>
#include <QTimer>
#include <QDateTime>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QChartView>
#include <QLineSeries>
#include <QRandomGenerator>
#include <QValueAxis>

extern int showsqlflag;
extern int savebehavepictureflag;
extern int savebehavedataflag;
extern int saveaccessdataflag;
extern int byte0;
extern int byte2;
extern int byte4;
extern int byte5;
extern int byte6;
extern QByteArray alldata;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_opentcpButton_clicked();

    void on_sendtcpButton_clicked();

    void on_closetcpButton_clicked();

    void on_desqlButton_clicked();

    void on_resqlButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

public slots:
    void showtimenow();
    void makedir();
    void maketxt();

    void opensql();
    void showsql();

    void initConnections();
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

    void initLinePlot1();
    void initLinePlot2();
    void initLinePlot3();
    void initLinePlot4();
    void updatelinePlot1();
    void updatelinePlot2();
    void updatelinePlot3();
    void updatelinePlot4();


private:
    Ui::MainWindow *ui;
    Registersql *registerui;
    Thread_sqlshow *thread_sql;
    QTimer *time;
    QStandardItemModel *model;
    QSortFilterProxyModel *proxymodel;
    QGraphicsScene *scene;
    QSqlDatabase db;
    QTcpServer *m_s;
    QTcpSocket *m_tcp;
    QByteArray m_receivedData;
    bool m_isReceivingImage = false;
    qint32 m_imageSize = 0;
    int m_imageType;

    //QTimer *datatime1;
    QChart *m_chart1;            // 图表对象
    QLineSeries *m_series1;
    QMap<qreal, qreal> m_visibleData1;
    const int VISIBLE_RANGE1 = 60;         // 数据系列
    int m_currentX1 = 0;

    //QTimer *datatime2;
    QChart *m_chart2;            // 图表对象
    QLineSeries *m_series2;
    QMap<qreal, qreal> m_visibleData2;
    const int VISIBLE_RANGE2 = 60;         // 数据系列
    int m_currentX2 = 0;

    //QTimer *datatime3;
    QChart *m_chart3;            // 图表对象
    QLineSeries *m_series3;
    QMap<qreal, qreal> m_visibleData3;
    const int VISIBLE_RANGE3 = 60;         // 数据系列
    int m_currentX3 = 0;

    //QTimer *datatime4;
    QChart *m_chart4;            // 图表对象
    QLineSeries *m_series4;
    QMap<qreal, qreal> m_visibleData4;
    const int VISIBLE_RANGE4 = 60;         // 数据系列
    int m_currentX4 = 0;

};
#endif // MAINWINDOW_H
