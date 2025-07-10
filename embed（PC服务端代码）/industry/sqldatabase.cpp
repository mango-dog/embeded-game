#include "mainwindow.h"

Thread_sqlshow::Thread_sqlshow()
{

}

void Thread_sqlshow::run()
{
    while(1)
    {
        if(showsqlflag == 1)
        {
            emit uishowsql();
        }
        else
        {}
        showsqlflag = 0;
        msleep(500);
    }
}
