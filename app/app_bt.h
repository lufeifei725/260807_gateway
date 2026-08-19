#if !defined(_APP_BT_H)
#define _APP_BT_H

#include "app_device.h"
typedef enum 
{
    BT_BR_9600 = '4',
    BT_BR_115200 = '8',
} BT_BaudRate;

int app_bt_init(Device *device);

int app_bt_preWrite(char *data, int len);
int app_bt_postRead(char *data, int len);


int app_bt_status(Device *device);

int app_bt_rename(Device *device, char *name);

int app_bt_setBaudRate(Device *device, BT_BaudRate baudRate);
int app_bt_reset(Device *device);
int app_bt_setNetId(Device *device, char *netId);
int app_bt_setMaddr(Device *device, char *maddr);


#endif // _APP_BT_H

