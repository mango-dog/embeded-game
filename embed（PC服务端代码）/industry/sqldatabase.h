#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QThread>
#include <QSlider>
#include <QMutex>

class Thread_sqlshow : public QThread
{
    Q_OBJECT
public:
    explicit Thread_sqlshow();

protected:
    void run();

signals:
    void uishowsql();

};

#endif // SQLDATABASE_H
