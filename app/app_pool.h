#if !defined(_APP_POOL_H)
#define _APP_POOL_H

typedef struct 
{
    int (*task_fun)(void *arg);
    void *arg;  
}Task;

int app_pool_init(int size);

void app_pool_destroy();

int app_pool_addTask(int (*task_fun)(void *arg), void *arg);

#endif // _APP_POOL_H
