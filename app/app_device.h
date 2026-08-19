#if !defined(_APP_DEVICE_H)
#define _APP_DEVICE_H
#include "app_buffer.h"
#include <pthread.h>

typedef struct
{
    char *filename;
    int fd;
    Buffer *up_buffer;
    Buffer *down_buffer;
    pthread_t read_thread;
    int is_running;
    int (*post_read)(char *date, int len);
    int (*pre_write)(char *date, int len);
    long last_write_time;
} Device;

Device *app_device_init(char *filename);
int app_device_start();   
void app_device_close();

#endif // _APP_DEVICE_H
