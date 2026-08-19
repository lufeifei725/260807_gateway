#include "app_buffer.h"
#include "log/log.h"
#include <stdlib.h>
#include <string.h>


static SubBuffer *init_sub_buffer(int size)
{
    SubBuffer *sub_buffer = (SubBuffer *)malloc(sizeof(SubBuffer));
    sub_buffer->ptr = (char *)malloc(size);
    sub_buffer->total_size = size;
    sub_buffer->len = 0;
    return sub_buffer;
}
Buffer *app_buffer_init(int size)
{
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    buffer->sub_buffers[0] = init_sub_buffer(size);
    buffer->sub_buffers[1] = init_sub_buffer(size);
    buffer->read_index = 0;
    buffer->write_index = 1;
    pthread_mutex_init(&buffer->read_lock, NULL);
    pthread_mutex_init(&buffer->write_lock, NULL);
    return buffer;
}

void app_buffer_free(Buffer *buffer)
{
    free(buffer->sub_buffers[0]->ptr);
    free(buffer->sub_buffers[1]->ptr);
    free(buffer->sub_buffers[0]);
    free(buffer->sub_buffers[1]);
    free(buffer);
}

int app_buffer_write(Buffer *buffer, char *data, int date_len)
{
    if (date_len > 255)
    {
        log_error("data is too long");
        return -1;
    }
    log_debug("add write_lock");
    pthread_mutex_lock(&buffer->write_lock);
    SubBuffer *w_buffer = buffer->sub_buffers[buffer->write_index];
    if (w_buffer->total_size - w_buffer->len < date_len + 1) 
    {
        log_error("buffer is full");
        pthread_mutex_unlock(&buffer->write_lock);
        return -1;
    }
    w_buffer->ptr[w_buffer->len] = date_len;
    memcpy(w_buffer->ptr + w_buffer->len+1, data, date_len);
    w_buffer->len += date_len + 1;
    log_debug("rm write_lock");
    pthread_mutex_unlock(&buffer->write_lock);
    return 0;
}

static void swap_sub_buffer(Buffer *buffer)
{
    log_debug("swap add write_lock");
    int temp = buffer->read_index;
    buffer->read_index = buffer->write_index;
    buffer->write_index = temp;
    log_debug("swap rm write_lock");
    pthread_mutex_unlock(&buffer->write_lock);
}
int app_buffer_read(Buffer *buffer, char *data_buf, int buf_size)
{
    log_debug("add read_lock");
    SubBuffer *r_buffer = buffer->sub_buffers[buffer->read_index];
    if (r_buffer->len == 0)
    {
        swap_sub_buffer(buffer);
        r_buffer = buffer->sub_buffers[buffer->read_index];
        if (r_buffer->len == 0)
        {
            log_error("buffer is empty");
            pthread_mutex_unlock(&buffer->read_lock);
            return -1;
        }
    }
    int data_len = r_buffer->ptr[0];
    log_debug("....data_len:%d", data_len);
    if (buf_size < data_len)
    {
        log_error("buffer is too small");
        pthread_mutex_unlock(&buffer->read_lock);
        return -1;
    }
    memcpy(data_buf, r_buffer->ptr + 1, data_len);
    memmove(r_buffer->ptr, r_buffer->ptr + data_len + 1, r_buffer->len - data_len - 1);
    r_buffer->len -= data_len + 1;
    log_debug("rm read_lock");
    pthread_mutex_unlock(&buffer->read_lock);
    return data_len;
}
