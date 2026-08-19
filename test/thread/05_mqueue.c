#define _GNU_SOURCE
#include "mqueue.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


static char *mq_name = "/my_mq";
static mqd_t mq_fd;
void *first_thread_fun(void *arg)
{
    sleep(1);
    printf("send message to queue\n");
    mq_send(mq_fd,"hello",5,0);
    return NULL;
}

void *second_thread_fun(void *arg)
{
    
    printf("second thread (%d) conduct\n",gettid());
    printf("receive message from queue\n");
    char buf[1024];
    ssize_t len = mq_receive(mq_fd,buf,1024,NULL);
    printf("receive message len = %ld,content = %s\n",len,buf);
    
    return NULL;
}
int main(int argc, char const *argv[])
{
    printf("main thread (%d) conduct\n",gettid());
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    mq_fd = mq_open(mq_name, O_RDWR | O_CREAT, 0644, &attr);
    if( mq_fd == -1)
    {
        perror("mq_open");
        return -1;
    }
    pthread_t pt1,pt2;
    pthread_create(&pt1,NULL,first_thread_fun,NULL);
    pthread_create(&pt2,NULL,second_thread_fun,NULL);
    printf("main thread (%d) continue\n",gettid());
    pthread_join(pt1,NULL);
    pthread_join(pt2,NULL);
    printf("main thread (%d) will close pt1 = %ld pt2 = %ld\n",gettid(),pt1,pt2);
    mq_close(mq_fd);
    mq_unlink(mq_name);
    return 0;
}
