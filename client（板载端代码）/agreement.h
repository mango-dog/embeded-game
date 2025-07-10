#ifndef AGREEMENT_H
#define AGREEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/serial.h>
#include <linux/can/raw.h>
#include <linux/can/netlink.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QObject>

class SerialPort:public QObject
{
    Q_OBJECT

public:
    SerialPort();
    ~SerialPort();

public:
    int OpenSerialPort(const char *device, const int baud);
    void SerialPortFlush(const int fd);
    void SerialClosePort(const int fd);
    int SerialAvailable(const int fd);
};

#endif // AGREEMENT_H
