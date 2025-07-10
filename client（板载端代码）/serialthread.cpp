#include "serialthread.h"
#include "mainwindow.h"
#include "agreement.h"

int n_read;
int tfp;

Thread_one::Thread_one()
{

}

void Thread_one::run()
{
    //QString look1;
    SerialPort s;

    while(1)
    {
        pause1.lock();
        n_read = s.SerialAvailable(fd);
        if(n_read>6)
        {
            tfp = read(fd,readbuf,n_read);
        }
        emit revice();
        msleep(1000);

        pause1.unlock();
    }
}
