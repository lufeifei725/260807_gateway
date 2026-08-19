#if !defined(_APP_SERIAL_H)
#define _APP_SERIAL_H
#include <termios.h>
#include "app_device.h"

typedef enum
{
    BR_9600 = B9600,
    BR_115200 = B115200,


} BaudRate;

typedef enum
{
    Parity_NONE = 0,
    Parity_ODD   = PARENB | PARODD,
    Parity_EVEN  = PARENB,
}Pariity;

typedef enum
{
    SB_1 = 0,
    SB_2 = CSTOPB,
}StopBits;
int app_serial_setBaudRate(Device *device, BaudRate braudRate);

int app_serial_setPariity(Device *device, Pariity parity);

int app_serial_setStopBits(Device *device, StopBits stopBits);

int app_serial_setBlock(Device *device, int block);

int app_serial_setRaw(Device *device);

int app_serial_init(Device *device);

int app_serial_flush(Device *device);

#endif // _APP_SERIAL_H
