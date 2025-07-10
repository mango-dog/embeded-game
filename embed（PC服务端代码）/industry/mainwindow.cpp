#include "mainwindow.h"
#include "ui_mainwindow.h"

QString currentTime;
QString showdata;
int showsqlflag;
int savebehavepictureflag;
int savebehavedataflag;
int saveaccessdataflag;
QByteArray alldata;
int byte0;
int byte2;
int byte4;
int byte5;
int byte6;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,registerui(nullptr)
{
    ui->setupUi(this);
    thread_sql = new Thread_sqlshow();

    //创建用于监听的服务器对象
    m_s=new QTcpServer(this);
    time = new QTimer(this);
    model = new QStandardItemModel(this);
    proxymodel = new QSortFilterProxyModel(this);


    ui->timelabel->setStyleSheet(
        "QLabel {"
        "   text-align: center;"
        "   padding-left: 0px;"
        "   padding-top: 3px;"
        "   padding-bottom: 0px;"
        "}"
        );
    ui->tableView->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    font: bold 14px 'Microsoft YaHei';"  // 字体加粗，12像素
        "    color: #333333;"                     // 字体颜色
        "    padding: 0px;"                       // 内边距
        "    border: 1px solid #cccccc;"          // 边框
        "    background-color: #f0f0f0;"          // 背景色
        "}"
        );

    setWindowTitle("多模态工业安全行为监测与智能调控系统");
    ui->portline->setText("8899");
    ui->ipline->setText("192.168.3.86");
    //ui->camera2->setAutoFillBackground(true);
    //ui->camera2->setStyleSheet("background-color: red;");
    ui->label_8->setAutoFillBackground(true);
    ui->label_8->setStyleSheet("background-color: green;");
    ui->label_9->setAutoFillBackground(true);
    ui->label_9->setStyleSheet("background-color: green;");
    ui->label_13->setAutoFillBackground(true);
    ui->label_13->setStyleSheet("background-color: green;");
    model->setHorizontalHeaderLabels({"序号", "门禁码", "备注", "是否使用", "创建时间"});
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setColumnWidth(0,60);
    ui->tableView->setColumnWidth(1,150);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->setColumnWidth(4,209);

    connect(time, &QTimer::timeout, this ,&MainWindow::showtimenow);
    connect(thread_sql, &Thread_sqlshow::uishowsql, this, &MainWindow::showsql);

    time->start(1000);
    thread_sql->start();
    initConnections();
    opensql();
    showsql();
    makedir();
    maketxt();
    initLinePlot1();
    initLinePlot2();
    initLinePlot3();
    initLinePlot4();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showtimenow()
{
    currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->timelabel->setText(currentTime);
}

void MainWindow::makedir()
{
    QDir dir("E:/embed/savebehave");
    if (!dir.exists()) {
        if (dir.mkdir("E:/embed/savebehave"))
        {
            qDebug() << "文件夹创建成功";
        } else
        {
            qDebug() << "文件夹创建失败";
        }
    }
}

void MainWindow::maketxt()
{
    QFile file("E:/embed/savebehave.txt");
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly))
        {
            file.close();
            qDebug() << "文件创建成功";
        } else
        {
            qDebug() << "文件创建失败:" << file.errorString();
        }
    }
    QFile file1("E:/embed/accesscontrol.txt");
    if (!file1.exists()) {
        if (file1.open(QIODevice::WriteOnly))
        {
            file1.close();
            qDebug() << "文件创建成功";
        } else
        {
            qDebug() << "文件创建失败:" << file1.errorString();
        }
    }
}

void MainWindow::opensql()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("control_card.db");
    if(!db.open())
    {
        qDebug() << "open sql error" << db.lastError();
    }
    //QSqlQuery query;
    /*QString sqlCreateTable = QString("create table control(id integer primary key autoincrement,"
                                     "conumber varchar(50),"
                                     "remarks varchar(30),"
                                     "usestate varchar(20),"
                                     "createtime varchar(100));");
    if(!query.exec(sqlCreateTable))
    {
        qDebug() << "create table error" << db.lastError();
    }*/
    /*QString sqlInsert = QString("insert into control(conumber,remarks,usestate,createtime) "
                                "values('HYW21','manager','Y','2025-06-24');");
    if(!query.exec(sqlInsert))
    {
        qDebug() << "create table error" << db.lastError();
    }*/
}

void MainWindow::showsql()
{
    model->removeRows(0, model->rowCount());
    QSqlQuery query;
    QString sqlselect = QString("select * from control;");
    if(!query.exec(sqlselect))
    {
        qDebug() << "select table error" << db.lastError();
    }
    while(query.next())
    {
        QString id = query.value("id").toString();
        QString conumber = query.value("conumber").toString();
        QString remarks = query.value("remarks").toString();
        QString usestate = query.value("usestate").toString();
        QString createtime = query.value("createtime").toString();
        //qDebug()<<id<<conumber<<remarks<<usestate<<createtime;
        QList<QStandardItem*> rowItems;
        for(const QString &text : {id, conumber, remarks, usestate, createtime})
        {
            QStandardItem *item = new QStandardItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            rowItems << item;
        }
        model->appendRow(rowItems);
    }
    proxymodel->setSourceModel(model);
    proxymodel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxymodel->setFilterKeyColumn(-1);
    ui->tableView->setModel(proxymodel);
    connect(ui->findlineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        proxymodel->setFilterFixedString(text);  // ÊµÊ±¹ýÂË
    });
}

void MainWindow::on_desqlButton_clicked()
{
    QModelIndex currentindex = ui->tableView->currentIndex();
    if(currentindex.isValid())
    {
        QModelIndex firstcolindex = currentindex.sibling(currentindex.row(), 0);
        QString value = firstcolindex.data().toString();
        qDebug()<<value;
        QSqlQuery query;
        QString sqldelete = QString("delete from control where id = %1;").arg(value.toInt());
        if(!query.exec(sqldelete))
        {
            qDebug() << "delete row error" << db.lastError();
        }
        else
        {
            //qDebug() << "delete row";
            query.exec(QString("UPDATE control SET id = id - 1 WHERE id > %1").arg(value.toInt()));
            query.exec("UPDATE sqlite_sequence SET seq = (SELECT MAX(id) FROM control) WHERE name='control'");
            //query.exec("UPDATE control SET id = (SELECT ROW_NUMBER() OVER (ORDER BY id));");
            //query.exec("DELETE FROM sqlite_sequence WHERE name='control';");
        }
    }
    else
    {
        qDebug()<<"no index";
    }
    showsqlflag = 1;
}

void MainWindow::on_resqlButton_clicked()
{
    if(!registerui)
    {
        registerui = new Registersql(this);
    }
    registerui->show();
}

void MainWindow::initConnections()
{
    connect(m_s, &QTcpServer::newConnection, this, &MainWindow::handleNewConnection);
}

void MainWindow::handleNewConnection()
{
    m_tcp = m_s->nextPendingConnection();
    connect(m_tcp, &QTcpSocket::readyRead, this, &MainWindow::handleReadyRead);
    connect(m_tcp, &QTcpSocket::disconnected, this, &MainWindow::handleDisconnected);
}

void MainWindow::handleReadyRead()
{
    QByteArray data = m_tcp->readAll();
    QString filename;

    if (data.startsWith("JPG:")) {
        // 开始接收图片（新协议格式："JPG:[类型1字节][大小8字节][数据...]"）
        m_isReceivingImage = true;

        // 提取图片类型（第4字节：'1'或'2'）
        m_imageType = data.mid(4,1).toInt();  // 转换为整数1或2

        // 提取图片大小（接下来的8字节）
        m_imageSize = data.mid(5,8).toInt();

        // 剩余数据（从第13字节开始）
        m_receivedData = data.mid(13);

        // 显示接收信息
        QString typeStr = (m_imageType == 1) ? "类型1" : "类型2";
        ui->receivetextEdit->append(QString("开始接收图片[%1]，大小: %2 bytes")
                                        .arg(typeStr)
                                        .arg(m_imageSize));

    } else if (m_isReceivingImage) {
        // 继续接收图片数据
        m_receivedData.append(data);

        if (m_receivedData.size() >= m_imageSize) {
            // 图片接收完成
            QPixmap pixmap;
            if (pixmap.loadFromData(m_receivedData)) {
                // 根据类型标志位处理不同图片
                if (m_imageType == 1) {
                    ui->camera1->setPixmap(pixmap.scaled(ui->camera1->size(),
                                                         Qt::KeepAspectRatio));
                    ui->receivetextEdit->append("类型1图片接收完成");
                    if(savebehavepictureflag == 1)
                    {
                        QString savePath = "E:/embed/savebehave/"; // 替换为您的实际路径
                        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
                        filename = savePath + "alert_" + timestamp + ".jpg";
                        pixmap.save(filename, "JPG");
                        /*if (pixmap.save(filename, "JPG")) {
                            ui->receivetextEdit->append("图片已保存至: " + filename);
                        } else {
                            ui->receivetextEdit->append("图片保存失败！");
                        }*/
                    }
                    else
                    {}
                    if(savebehavedataflag == 1)
                    {
                        QFile logFile("E:/embed/savebehave.txt");
                        if (logFile.open(QIODevice::Append | QIODevice::Text))
                        {
                            QTextStream logStream(&logFile);
                            logStream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                      << " 未执行安全规范图片保存 "
                                      << filename << "\n";
                            logFile.close();
                        }
                    }
                    else
                    {}
                }
                else
                {  // m_imageType == 2
                    ui->camera2->setPixmap(pixmap.scaled(ui->camera2->size(),
                                                         Qt::KeepAspectRatio));
                    ui->receivetextEdit->append("类型2图片接收完成");
                }
            } else {
                ui->receivetextEdit->append("图片解析失败！");
            }

            // 重置状态
            m_isReceivingImage = false;
            m_receivedData.clear();
        }
    }
    else
    {
        // 普通文本消息
        ui->receivetextEdit->append("客户端say：" + QString::fromUtf8(data));
        //showdata = data.toHex();
        //qDebug()<<data;
        alldata = data;
        QByteArray abc = QByteArray::fromHex(alldata);
        byte0 = static_cast<quint8>(abc.at(0));
        byte2 = static_cast<quint8>(abc.at(2));
        byte4 = static_cast<quint8>(abc.at(4));
        byte5 = static_cast<quint8>(abc.at(5));
        byte6 = static_cast<quint8>(abc.at(6));
        updatelinePlot1();
        updatelinePlot2();
        updatelinePlot3();
        updatelinePlot4();
    }
}

void MainWindow::handleDisconnected()
{
    m_tcp->close();
    m_tcp->deleteLater();
    ui->receivetextEdit->append("客户端已连接");
}

void MainWindow::on_opentcpButton_clicked()
{
    unsigned short port=ui->portline->text().toUShort();//获取端口号
    m_s->listen(QHostAddress::Any,port);//启动监听
    ui->opentcpButton->setEnabled(false);
    ui->receivetextEdit->append("服务器开始监听");
}

void MainWindow::on_closetcpButton_clicked()
{
    if (m_s && m_s->isListening())
    {
        m_s->close();  // 关闭监听
        ui->opentcpButton->setEnabled(true);  // 重新启用打开按钮
        ui->receivetextEdit->append("服务器已停止监听");
    }
    else
    {}
}

void MainWindow::on_sendtcpButton_clicked()
{
    //QString msg=ui->textEdit_2->toPlainText();
    //m_tcp->write(msg.toUtf8());
    //ui->receivetextEdit->append("服务器say："+msg);
    QPixmap pixmap("E:/embed/savebehave/alert_20250630_144944_156.jpg");
    ui->camera2->setPixmap(pixmap);
    ui->textEdit_3->append("识别到门禁图片帧！！");
    ui->textEdit_3->append("提取到图片数据为：XXXXX");
    ui->textEdit_3->append("识别成功！");
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        savebehavepictureflag = 1;
    }
    else if (arg1 == Qt::Unchecked)
    {
        savebehavepictureflag = 0;
    }
}


void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        savebehavedataflag = 1;
    }
    else if (arg1 == Qt::Unchecked)
    {
        savebehavedataflag = 0;
    }
}


void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        saveaccessdataflag = 1;
    }
    else if (arg1 == Qt::Unchecked)
    {
        saveaccessdataflag = 0;
    }
}

void MainWindow::initLinePlot1()
{
    m_chart1 = new QChart();
    ui->graphicsView->setChart(m_chart1);
    m_chart1->setMargins(QMargins(0, 0, 0, 0));  // 去除边距
    m_chart1->legend()->hide();

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 60);
    axisX->setGridLineVisible(false);
    m_chart1->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-40, 40);
    axisY->setGridLineVisible(false);
    m_chart1->addAxis(axisY, Qt::AlignLeft);

    m_series1 = new QLineSeries();
    m_chart1->addSeries(m_series1);
    m_series1->attachAxis(axisX);
    m_series1->attachAxis(axisY);

    /*datatime1 = new QTimer(this);
    connect(datatime1, &QTimer::timeout,
            this, &MainWindow::updatelinePlot1);
    datatime1->start(1000);*/
}

void MainWindow::initLinePlot2()
{
    m_chart2 = new QChart();
    ui->graphicsView_2->setChart(m_chart2);
    m_chart2->setMargins(QMargins(0, 0, 0, 0));  // 去除边距
    m_chart2->legend()->hide();

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 60);
    axisX->setGridLineVisible(false);
    m_chart2->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 100);
    axisY->setGridLineVisible(false);
    m_chart2->addAxis(axisY, Qt::AlignLeft);

    m_series2 = new QLineSeries();
    m_chart2->addSeries(m_series2);
    m_series2->attachAxis(axisX);
    m_series2->attachAxis(axisY);

    /*datatime2 = new QTimer(this);
    connect(datatime2, &QTimer::timeout,
            this, &MainWindow::updatelinePlot2);
    datatime2->start(1000);*/
}

void MainWindow::initLinePlot3()
{
    m_chart3 = new QChart();
    ui->graphicsView_3->setChart(m_chart3);
    m_chart3->setMargins(QMargins(0, 0, 0, 0));  // 去除边距
    m_chart3->legend()->hide();

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 60);
    axisX->setGridLineVisible(false);
    m_chart3->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-2, 2);
    axisY->setGridLineVisible(false);
    m_chart3->addAxis(axisY, Qt::AlignLeft);

    m_series3 = new QLineSeries();
    m_chart3->addSeries(m_series3);
    m_series3->attachAxis(axisX);
    m_series3->attachAxis(axisY);


}

void MainWindow::initLinePlot4()
{
    m_chart4 = new QChart();
    ui->graphicsView_4->setChart(m_chart4);
    m_chart4->setMargins(QMargins(0, 0, 0, 0));  // 去除边距
    m_chart4->legend()->hide();

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 60);
    axisX->setGridLineVisible(false);
    m_chart4->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 3000);
    axisY->setGridLineVisible(false);
    m_chart4->addAxis(axisY, Qt::AlignLeft);

    m_series4 = new QLineSeries();
    m_chart4->addSeries(m_series4);
    m_series4->attachAxis(axisX);
    m_series4->attachAxis(axisY);


}

void MainWindow::updatelinePlot1()
{
    /*if (m_currentX1 > 200) {
        datatime1->stop();
        return;
    }*/
    qreal randomDecimal = QRandomGenerator::global()->bounded(0, 100) / 100.0;
    qreal yValue = byte0 + randomDecimal;
    m_series1->append(m_currentX1, yValue);

    m_visibleData1[m_currentX1] = yValue;

    if (m_currentX1 > VISIBLE_RANGE1) {
        qreal removeX = m_currentX1 - VISIBLE_RANGE1;
        m_visibleData1.remove(removeX);

        // 也可以选择清理series中不可见的数据（可选）
        if (m_series1->count() > VISIBLE_RANGE1 * 1.5) {
            m_series1->removePoints(0, m_series1->count() - VISIBLE_RANGE1);
        }
    }

    // 自动滚动视图
    if (m_currentX1 > VISIBLE_RANGE1) {  // 当超过50个点时开始滚动
        m_chart1->axisX()->setRange(m_currentX1 - VISIBLE_RANGE1, m_currentX1);
    }

    if (m_visibleData1.isEmpty()) return;
    auto maxIt = std::max_element(m_visibleData1.begin(), m_visibleData1.end());
    qreal max = *maxIt;
    QString maxdata1 = QString::number(max,'f',2);
    qreal avg = std::accumulate(m_visibleData1.begin(), m_visibleData1.end(), 0.0)
                / m_visibleData1.size();
    if(avg >= 40)
    {
        ui->label_8->setStyleSheet("background-color: red;");
    }
    QString avgdata1 = QString::number(avg,'f',2);
    ui->lineEdit->setText(maxdata1);
    ui->lineEdit_2->setText(avgdata1);

    m_currentX1=m_currentX1+2;
}

void MainWindow::updatelinePlot2()
{
    qreal randomDecimal = QRandomGenerator::global()->bounded(0, 100) / 100.0;
    qreal yValue = byte2+randomDecimal;
    QString shidu = QString::number(yValue,'f',2);
    ui->lineEdit_3->setText(shidu);
    //if(byte0 <30 || byte0 >70)
    //{
       // ui->label_9->setStyleSheet("background-color: red;");
    //}
    m_series2->append(m_currentX2, yValue);

    m_visibleData2[m_currentX2] = yValue;

    if (m_currentX2 > VISIBLE_RANGE2) {
        qreal removeX = m_currentX2 - VISIBLE_RANGE2;
        m_visibleData2.remove(removeX);

        // 也可以选择清理series中不可见的数据（可选）
        if (m_series2->count() > VISIBLE_RANGE2 * 1.5) {
            m_series2->removePoints(0, m_series2->count() - VISIBLE_RANGE2);
        }
    }

    // 自动滚动视图
    if (m_currentX2 > VISIBLE_RANGE2) {  // 当超过50个点时开始滚动
        m_chart2->axisX()->setRange(m_currentX2 - VISIBLE_RANGE2, m_currentX2);
    }

    /*if (m_visibleData2.isEmpty()) return;
    auto maxIt = std::max_element(m_visibleData2.begin(), m_visibleData2.end());
    qreal max = *maxIt;
    QString maxdata1 = QString::number(max,'f',2);
    qreal avg = std::accumulate(m_visibleData1.begin(), m_visibleData1.end(), 0.0)
                / m_visibleData1.size();
    QString avgdata1 = QString::number(avg,'f',2);
    ui->lineEdit->setText(maxdata1);
    ui->lineEdit_2->setText(avgdata1);*/

    m_currentX2=m_currentX2+2;
}

void MainWindow::updatelinePlot3()
{
    if(byte4 == 1)
    {
        byte4 = 0;
    }
    else
    {
        byte4 = 1;
    }
    if(byte4 == 1)
    {
        ui->label_13->setStyleSheet("background-color: red;");
    }
    if(byte4 == 0)
    {
        ui->label_13->setStyleSheet("background-color: green;");
    }
    qreal yValue = 0;
    m_series3->append(m_currentX3, yValue);

    m_visibleData3[m_currentX3] = yValue;

    if (m_currentX3 > VISIBLE_RANGE3) {
        qreal removeX = m_currentX3 - VISIBLE_RANGE3;
        m_visibleData3.remove(removeX);

        // 也可以选择清理series中不可见的数据（可选）
        if (m_series3->count() > VISIBLE_RANGE3 * 1.5) {
            m_series3->removePoints(0, m_series3->count() - VISIBLE_RANGE3);
        }
    }

    // 自动滚动视图
    if (m_currentX3 > VISIBLE_RANGE3) {  // 当超过50个点时开始滚动
        m_chart3->axisX()->setRange(m_currentX3 - VISIBLE_RANGE3, m_currentX3);
    }

    /*if (m_visibleData2.isEmpty()) return;
    auto maxIt = std::max_element(m_visibleData2.begin(), m_visibleData2.end());
    qreal max = *maxIt;
    QString maxdata1 = QString::number(max,'f',2);
    qreal avg = std::accumulate(m_visibleData1.begin(), m_visibleData1.end(), 0.0)
                / m_visibleData1.size();
    QString avgdata1 = QString::number(avg,'f',2);
    ui->lineEdit->setText(maxdata1);
    ui->lineEdit_2->setText(avgdata1);*/

    m_currentX3=m_currentX3+2;
}

void MainWindow::updatelinePlot4()
{
    std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));

    // 定义分布范围 [2500, 3000]
    std::uniform_int_distribution<int> dist(2500, 3000);

    // 生成并输出随机数
    int random_number = dist(gen);
    qreal yValue = random_number;
    m_series4->append(m_currentX4, yValue);

    m_visibleData4[m_currentX4] = yValue;

    if (m_currentX4 > VISIBLE_RANGE4) {
        qreal removeX = m_currentX4 - VISIBLE_RANGE4;
        m_visibleData4.remove(removeX);

        // 也可以选择清理series中不可见的数据（可选）
        if (m_series4->count() > VISIBLE_RANGE4 * 1.5) {
            m_series4->removePoints(0, m_series4->count() - VISIBLE_RANGE4);
        }
    }

    // 自动滚动视图
    if (m_currentX4 > VISIBLE_RANGE4) {  // 当超过50个点时开始滚动
        m_chart4->axisX()->setRange(m_currentX4 - VISIBLE_RANGE4, m_currentX4);
    }

    /*if (m_visibleData2.isEmpty()) return;
    auto maxIt = std::max_element(m_visibleData2.begin(), m_visibleData2.end());
    qreal max = *maxIt;
    QString maxdata1 = QString::number(max,'f',2);
    qreal avg = std::accumulate(m_visibleData1.begin(), m_visibleData1.end(), 0.0)
                / m_visibleData1.size();
    QString avgdata1 = QString::number(avg,'f',2);
    ui->lineEdit->setText(maxdata1);
    ui->lineEdit_2->setText(avgdata1);*/

    m_currentX4=m_currentX4+2;
}
