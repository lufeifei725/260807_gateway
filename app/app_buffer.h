#if !defined(__APP_BUFFER_H)
#define __APP_BUFFER_H
#include <pthread.h>
typedef struct
{
    unsigned char *ptr;
    int total_size;
    int len;
   
}SubBuffer; 

typedef struct
{
    SubBuffer *sub_buffers[2];
    int read_index;
    int write_index;
    pthread_mutex_t read_lock;
    pthread_mutex_t write_lock;
}Buffer;

Buffer *app_buffer_init(int size);

void app_buffer_free(Buffer *buffer);

int app_buffer_write(Buffer *buffer, char *data, int date_len);

int app_buffer_read(Buffer *buffer, char *data_buf, int buf_size);


#endif // __APP_BUFFER_H
