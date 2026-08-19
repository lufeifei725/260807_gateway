#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


void *first_thread_fun(void *arg)
{
    printf("first thread (%d) conduct\n",gettid());
    return NULL;
}

void *second_thread_fun(void *arg)
{
    printf("second thread (%d) conduct\n",gettid());
    return NULL;
}
int main(int argc, char const *argv[])
{
    printf("main thread (%d) conduct\n",gettid());
    pthread_t pt1,pt2;
    pthread_create(&pt1,NULL,first_thread_fun,NULL);
    pthread_create(&pt2,NULL,second_thread_fun,NULL);
    printf("main thread (%d) continue\n",gettid());
    pthread_join(pt1,NULL);
    pthread_join(pt2,NULL);
    printf("main thread (%d) will close pt1 = %ld pt2 = %ld\n",gettid(),pt1,pt2);
    return 0;
}
