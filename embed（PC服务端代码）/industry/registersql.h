#ifndef REGISTERSQL_H
#define REGISTERSQL_H

#include <QMainWindow>

namespace Ui {
class Registersql;
}

class Registersql : public QMainWindow
{
    Q_OBJECT

public:
    explicit Registersql(QWidget *parent = nullptr);
    ~Registersql();

private slots:
    void on_closeButton_clicked();

    void on_enterButton_clicked();

private:
    Ui::Registersql *ui;
};

#endif // REGISTERSQL_H
