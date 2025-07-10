#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QThread>
#include <QSlider>
#include <QMutex>

extern int n_read;

class Thread_one : public QThread
{
    Q_OBJECT
public:
    explicit Thread_one();
    int Fd;
    void setFd(int *fd){Fd = *fd;};
    QMutex pause1;

protected:
    void run();

signals:
    void revice();
};

#endif // SERIALTHREAD_H
