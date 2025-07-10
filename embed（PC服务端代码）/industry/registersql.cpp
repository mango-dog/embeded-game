#include "registersql.h"
#include "mainwindow.h"
#include "ui_registersql.h"

Registersql::Registersql(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Registersql)
{
    ui->setupUi(this);
    ui->comboBox->setStyleSheet(
        "QComboBox {"
        "    text-align: center;"  // 文本居中
        "    padding-left: 10px;"  // 可选：调整左侧内边距
        "}"
        "QComboBox QAbstractItemView {"
        "    text-align: center;"  // 下拉项的文本也居中
        "}"
        );
    ui->comboBox_2->setStyleSheet(
        "QComboBox {"
        "    text-align: center;"  // 文本居中
        "    padding-left: 60px;"  // 可选：调整左侧内边距
        "}"
        "QComboBox QAbstractItemView {"
        "    text-align: center;"  // 下拉项的文本也居中
        "}"
        );
    ui->comboBox_3->setStyleSheet(
        "QComboBox {"
        "    text-align: center;"  // 文本居中
        "    padding-left: 25px;"  // 可选：调整左侧内边距
        "}"
        "QComboBox QAbstractItemView {"
        "    text-align: center;"  // 下拉项的文本也居中
        "}"
        );
    ui->comboBox_4->setStyleSheet(
        "QComboBox {"
        "    text-align: center;"  // 文本居中
        "    padding-left: 40px;"  // 可选：调整左侧内边距
        "}"
        "QComboBox QAbstractItemView {"
        "    text-align: center;"  // 下拉项的文本也居中
        "}"
        );
    ui->comboBox_5->setStyleSheet(
        "QComboBox {"
        "    text-align: center;"  // 文本居中
        "    padding-left: 40px;"  // 可选：调整左侧内边距
        "}"
        "QComboBox QAbstractItemView {"
        "    text-align: center;"  // 下拉项的文本也居中
        "}"
        );
}

Registersql::~Registersql()
{
    delete ui;
}

void Registersql::on_closeButton_clicked()
{
    this->close();
}


void Registersql::on_enterButton_clicked()
{
    QString addconumber = ui->lineEdit->text();
    QString addremarks = ui->comboBox->currentText();
    QString addusestate = ui->comboBox_2->currentText();
    QString addyear = ui->comboBox_3->currentText();
    QString addmonth = ui->comboBox_4->currentText();
    QString addday = ui->comboBox_5->currentText();
    QString addtime = addyear + "-" + addmonth + "-" + addday;
    //qDebug()<<addconumber;
    //qDebug()<<addremarks;
    //qDebug()<<addusestate;
    //qDebug()<<addtime;
    QSqlQuery query;
    query.prepare("INSERT INTO control(conumber, remarks, usestate, createtime) "
                  "VALUES (:conumber, :remarks, :usestate, :createtime)");
    query.bindValue(":conumber", addconumber);
    query.bindValue(":remarks", addremarks);
    query.bindValue(":usestate", addusestate);
    query.bindValue(":createtime", addtime);
    if(!query.exec())
    {
        qDebug() << "create table error" << query.lastError();
    }
    showsqlflag = 1;
    this->close();
}

