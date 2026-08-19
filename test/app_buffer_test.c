#include "app_buffer.h"
#include "log/log.h"


int main(int argc, char const *argv[])
{
    Buffer *buffer0 = app_buffer_init(1000);
    char data[255];
    for (int i = 0; i < 255; i++)
    {
        data[i] = 'a';
    }
    app_buffer_write(buffer0, data, 255);
    char data_buf0[300];
    int data_len0 = app_buffer_read(buffer0, data_buf0, 300);
    log_debug("read data0: %.*s,length: %d",data_len0,data_buf0,data_len0);
    
    
    Buffer *buffer = app_buffer_init(13);
    app_buffer_write(buffer, "abcde", 5);
    app_buffer_write(buffer, "fghijk", 6);

    char data_buf[10];
    int data_len = app_buffer_read(buffer, data_buf, 10);
    log_debug("read data1: %.*s,length: %d",data_len,data_buf,data_len);
    char data_buf2[10];
    int data_len2 = app_buffer_read(buffer, data_buf2, 10);
    log_debug("read data2: %.*s,length: %d",data_len2,data_buf2,data_len2);
    char data_buf3[10];
    int data_len3 = app_buffer_read(buffer, data_buf3, 10);
    log_debug("read data3: %.*s,length: %d",data_len3,data_buf3,data_len3);

    
    app_buffer_write(buffer, "123131231312312312312313", 19);
    app_buffer_free(buffer);
    return 0;
}
