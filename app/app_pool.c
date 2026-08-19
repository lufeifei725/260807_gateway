#include "app_pool.h"
#include "log/log.h"
#include <pthread.h>
#include <stdlib.h>
#include <mqueue.h>
static int thread_num;
static pthread_t *thread_pool;
static  mqd_t mq_fd;
static char *mq_name = "/app_pool_mq";
void *thread_fun(void *arg)
{
    Task task;
    while (1)
    {
        int len = mq_receive(mq_fd, (char *)&task, sizeof(Task), NULL);
        if (len == sizeof(Task))
        {
            task.task_fun(task.arg);
        }
        else
        {
            log_error("mq_receive error");
            break;    
        }

    }
}
int app_pool_init(int size)
{
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Task);
    mq_fd = mq_open(mq_name, O_RDWR | O_CREAT, 0644, &attr);
    if (mq_fd == -1)
    {
        log_error("mq_open error");
        return -1;
    }
    
    thread_num = size;
    thread_pool = malloc(sizeof(pthread_t) * size);
    for (int i = 0; i < size; i++)
    {
        pthread_create(&thread_pool[i], NULL, thread_fun, NULL);
    }
    log_error("app_pool_init success");
    return 0;
}


void app_pool_destroy()
{
    mq_close(mq_fd);
    mq_unlink(mq_name);
    for (int i = 0; i < thread_num; i++)
    {
        pthread_cancel(thread_pool[i]);
        pthread_join(thread_pool[i], NULL);
    }
    free(thread_pool);
}

int app_pool_addTask(int (*task_fun)(void *arg), void *arg)
{
    Task task = {
        .task_fun = task_fun,
        .arg = arg,
    };
    if (mq_send(mq_fd, (char *)&task, sizeof(Task), 0) == -1)
    {
        log_error("mq_send error");
        return -1;
    }
}