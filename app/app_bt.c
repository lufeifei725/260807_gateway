#define _GNU_SOURCE
#include <string.h>
#include "app_bt.h"
#include "log/log.h"
#include "app_serial.h"


static int init_bt(Device *device)
{
    app_serial_init(device);
    app_serial_setBlock(device,0);
    app_serial_flush(device);
    if (app_bt_status(device) ==0)
    {
        app_bt_setBaudRate(device,BT_BR_115200);
        app_bt_reset(device);
        sleep(2);
    }
    app_serial_setBaudRate(device,BR_115200);
    app_serial_flush(device);
    if (app_bt_status(device) !=0)
    {
        log_error("bt init fail");
        return -1;
    }
    app_bt_setNetId(device,"1234");
    app_bt_setMaddr(device,"0101");
    app_serial_setBlock(device,1);
    app_serial_flush(device);
    log_debug("bt init success");
    return 0;

}
int app_bt_init(Device *device)
{
    device->pre_write = app_bt_preWrite;
    device->post_read = app_bt_postRead;
}

int app_bt_preWrite(char *data, int data_len)
{
    if(data_len < 6)
    {
        log_error("data_len < 6");
        return -1;
    }
    int blue_len = 8 + 2 + data[2] +2;
    char blue_data[blue_len];
    memcpy(blue_data,"AT+MESH",8);
    memcpy(blue_data+8,data+3,2);
    memcpy(blue_data+10,data+5,data[2]);
    memcpy(blue_data+10+data[2],"\r\n",2);
    memset(data,0,data_len);
    memcpy(data,blue_data,blue_len);
    return blue_len;
}

static char read_buf[1024];
static int read_len = 0;
static char fix_header[2] = {0xf1,0xdd};


static void remove_data(int len)
{
    memmove(read_buf,read_buf+len,read_len-len);
    read_len -= len;
}
int app_bt_postRead(char *data, int data_len)
{
    memcpy(read_buf+read_len,data,data_len);
    read_len += data_len;
    if(read_len < 8)
    {
        log_debug("read_len < 8");
        return 0;
    }
    int i;
    for (i = 0; i < read_len - 7; i++)
    {
        if(memcmp(fix_header,read_buf+i,2) == 0)
        {
            if (i > 0)
            {
                remove_data(i);
                if (read_len < 8)
                {
                    log_debug("2read_len < 8");
                    return 0;
                }
            }
            int blue_len = read_buf[2] + 3;
            if (read_len < blue_len)
            {
                log_debug("blue_len is not enough continue read");
                return 0;
            }
            memset(data,0,data_len);
            data[0] = 1;
            data[1] = 2;
            data[2] = blue_len - 7;
            memcpy(data+3,read_buf+3,2);
            memcpy(data+5,read_buf+7,data[2]);
            remove_data(blue_len);
            return 5 + data[2];

        }
    }
    remove_data(i);
    
    return 0;
}

int app_bt_init(Device *device)
{
    device->pre_write = app_bt_preWrite;
    device->post_read = app_bt_postRead;

}

int app_bt_preWrite(char *data, int len);
int app_bt_postRead(char *data, int len);

int wait_ack(int fd)
{
    usleep(50*1000);
    char data_buf[4];
    read(fd,data_buf,4);
    if(memcmp(data_buf,"OK\r\n",4) != 1)
    {
        log_error("wait ack error");
        return -1;
    }
    return 0;
}
int app_bt_status(Device *device)
{
    write(device->fd,"AT\r\n",4);
    return wait_ack(device->fd);

}

int app_bt_rename(Device *device, char *name)
{
    char cmd[20];
    sprintf(cmd,"AT+NAME%s\r\n",name);
    write(device->fd,cmd,strlen(cmd));
    return wait_ack(device->fd);
}

int app_bt_setBaudRate(Device *device, BT_BaudRate baudRate)
{
    char cmd[20];
    sprintf(cmd,"AT+BAUD%d\r\n",baudRate);
    write(device->fd,cmd,strlen(cmd));
    return wait_ack(device->fd);
}
int app_bt_reset(Device *device)
{
    char *cmd = "AT+RESET\r\n";
    write(device->fd,cmd,strlen(cmd));
    return wait_ack(device->fd);
}
int app_bt_setNetId(Device *device, char *netid)
{
    char cmd[20];
    sprintf(cmd,"AT+NETID%s\r\n",netid);
    write(device->fd,cmd,strlen(cmd));
    return wait_ack(device->fd);

}
int app_bt_setMaddr(Device *device, char *maddr)
{
    char cmd[20];
    sprintf(cmd,"AT+MADDR%s\r\n",maddr);
    write(device->fd,cmd,strlen(cmd));
    return wait_ack(device->fd);
}