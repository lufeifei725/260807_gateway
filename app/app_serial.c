#define _GNU_SOURCE
#include "app_serial.h"
#include "log/log.h"

int app_serial_setBaudRate(Device *device, BaudRate braudRate)
{
    struct termios attr;
    tcgetattr(device->fd, &attr);
    cfsetspeed(&attr, braudRate);
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result == -1)
    {
        log_error("Failed to set baud rate");
        return -1;
    }
    log_debug("Baud rate set sucess");
    return 0;


}

int app_serial_setPariity(Device *device, Pariity parity)
{
    struct termios attr;
    tcgetattr(device->fd, &attr);
    attr.c_cflag &= ~(PARENB | PARODD); 
    cfsetspeed(&attr, parity);
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result == -1)
    {
        log_error("Failed to set parity");
        return -1;
    }
    log_debug("Parity set sucess");
    return 0;
}

int app_serial_setStopBits(Device *device, StopBits stopBits)
{
    struct termios attr;
    tcgetattr(device->fd, &attr);
    attr.c_cflag &= ~CSTOPB;
    cfsetspeed(&attr, stopBits);
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result == -1)
    {
        log_error("Failed to set stop bits");
        return -1;
    }
    log_debug("Stop bits set sucess");
    return 0;

}

int app_serial_setBlock(Device *device, int is_block)
{
    struct termios attr;
    tcgetattr(device->fd, &attr);
    if(is_block==1)
    {
        attr.c_cc[VMIN] = 1;
        attr.c_cc[VTIME] = 0;
    }else
    {
        attr.c_cc[VMIN] = 0;
        attr.c_cc[VTIME] = 2;
    }
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result == -1)
    {
        log_error("Failed to set block");
        return -1;
    }
    log_debug("Block set sucess");
    return 0;
}
   

int app_serial_setRaw(Device *device)
{
    struct  termios attr;
    tcgetattr(device->fd, &attr);
    cfmakeraw(&attr);
    int result = tcsetattr(device->fd, TCSAFLUSH, &attr);
    if (result == -1)
    {
        log_error("Failed to set raw");
        return -1;
    }
    log_debug("Raw set sucess");
    return 0;
}
   
int app_serial_init(Device *device)
{
    app_serial_setBaudRate(device, BR_9600);
    app_serial_setPariity(device, Parity_NONE);
    app_serial_setStopBits(device, SB_1);
    // app_serial_setBlock(device, 0);
    app_serial_setRaw(device);
    int result = tcflush(device->fd,TCIOFLUSH);
    if (result == -1)
    {
        log_error("Failed to flush");
        return -1;
    }
    log_debug("Serial init sucess");
    return 0;
}

int app_serial_flush(Device *device)
{
    return tcflush(device->fd,TCIOFLUSH);
}