#define _GNU_SOURCE
#include "app_device.h"
#include "app_common.h"
#include "app_mqtt.h"
#include "app_message.h"
#include "log/log.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "app_pool.h"


#define BUFFER_SIZE 1024
Device *device;

static int send_msg_fun(void *arg)
{
    char data_buf[128];
    int data_len = app_buffer_read(device->up_buffer,data_buf,sizeof(data_buf));
    char *json = app_message_chars2Json(data_buf,data_len);
    int result = app_mqtt_send(json);
    if (result == -1)
    {
        log_error("mqtt send error");
        return -1;
    }
    log_debug("mqtt send success");

}

static void *read_thread_fun(void *arg)
{
    while (device->is_running)
    {
        char data_buf[128];
        ssize_t data_len = read(device->fd,data_buf,sizeof(data_buf));
        // log_debug("--------:%d",data_len);
        if (data_len > 0 && device->post_read)
        {
            data_len = device->post_read(data_buf,data_len);
        }
        if (data_len > 0)
        {
            app_buffer_write(device->up_buffer,data_buf,data_len);
            app_pool_addTask(send_msg_fun,NULL);
        }
    }
            
    
    
}
Device *app_device_init(char *filename)
{
    if (device)
    {
        return device;
    }
    device = (Device *)malloc(sizeof(Device));
    device->fd = open(filename, O_RDWR);
    device->up_buffer = app_buffer_init(BUFFER_SIZE);
    device->down_buffer = app_buffer_init(BUFFER_SIZE);
    device->is_running = 0;
    device->post_read = NULL;
    device->pre_write = NULL;
    app_pool_init(5);
    app_mqtt_init();
    return device;
}
static int write_task_fun(void *arg)
{
    Device *device = (Device *)arg;
    char data_buf[128];
    int data_len = app_buffer_read(device->down_buffer,data_buf,sizeof(data_buf));
    if (device->pre_write)
    {
        data_len = device->pre_write(data_buf,data_len);

    }
    long distance = app_common_getCurrentTime() - device->last_write_time;
    if (distance < 200)
    {
        usleep((200 - distance) * 1000);
    }
    ssize_t len = write(device->fd,data_buf,data_len);
    if (len != data_len)
    {
        log_error("write bluetoose error");
        return -1;
    }
    log_debug("write bluutoose success:%.*s",data_len,data_buf);
    device->last_write_time = app_common_getCurrentTime();
    return 0;
}
static int rec_msg_callback(char *json)
{
    char data_buf[128];
    int data_len = app_message_json2Chars(json,data_buf,sizeof(data_buf));
    app_buffer_write(device->down_buffer,data_buf,data_len);
    app_pool_addTask(write_task_fun,device);
}
int app_device_start()
{
    if (device->is_running)
    {
        log_debug("device is running");
        return 0;
    }
    device->is_running = 1;
    pthread_create(&device->read_thread, NULL, read_thread_fun, NULL);
    app_mqtt_registerReceiveCallback(rec_msg_callback);


}   
void app_device_close()
{
    close(device->fd);
    app_buffer_free(device->up_buffer);
    app_buffer_free(device->down_buffer);
    pthread_cancel(device->read_thread);
    pthread_join(device->read_thread,NULL);
    free(device);
    app_pool_destroy();
    app_mqtt_close();
}
