#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#define NUM 1000000
int n1=0;
int*sum;
void * worker_func(void *arg)
{
    int id = (int)arg;
    double x, y;
    int n;
    srand((unsigned)time(NULL));
    for (n = 1, sum[id] = 0; n <= NUM/n1; n++)
    {
        x = rand() / (double)RAND_MAX;
        y = rand() / (double)RAND_MAX;
        if ((x*x+y*y) <= 1)
        {
            sum[id]++;
        }
    }
    return NULL;
}


int main(int argc,char* argv[])
{
    int i=0;
    while(argv[2][i]!='\0'){
        n1=(n1*10)+argv[2][i]-'0';
        i++;
    }
    pthread_t* worker=(pthread_t*)malloc(sizeof(pthread_t)*n1);
    sum=(int *)malloc(sizeof(int)*n1);
    void **worker_status=(void**)malloc(sizeof(void*)*n1);
    i=0;
    while(i<n1){
        pthread_create(&worker[i], NULL, worker_func, (void*) i);
        i++;
    }
    
    i=0;
    while(i<n1){
        pthread_join(worker[i], &worker_status[i]);
    i++;
    }
    int k=0;
    for(i=0;i<n1;i++)
        k+=sum[i];
    printf("pi=%7.7lf\n", 4*(double)k/(double)NUM);

    return 0;
}
