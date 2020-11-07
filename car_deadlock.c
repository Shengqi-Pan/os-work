#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

/* 声明abcd四个资源 */
int resource_a,
    resource_b,
    resource_c,
    resource_d;

/* 声明abcd四个互斥锁用于保护四个资源 */
pthread_mutex_t mutex_a,
                mutex_b,
                mutex_c,
                mutex_d;

/* 声明四个信号量用作计数器 */
sem_t sem_north,
      sem_east,
      sem_south,
      sem_west;

/* 声明输入队列 */
char incoming_queue[] = "nsewwewn";

/* 声明线程 */
void *thread_north(void *arg);
void *thread_east(void *arg);
void *thread_south(void *arg);
void *thread_west(void *arg);

/* main函数 */
int main(void)
{
    int res;
    pthread_t thread_1, thread_2, thread_3, thread_4;  // 新建一个thread indetenfier
    
    /* 初始化互斥锁 */
    res = 0;
    res += pthread_mutex_init(&mutex_a, NULL);
    res += pthread_mutex_init(&mutex_b, NULL);
    res += pthread_mutex_init(&mutex_c, NULL);
    res += pthread_mutex_init(&mutex_d, NULL);
    if (res != 0)
    {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    /* 初始化信号量 */
    // 计数信号量全部初始化为0，因为一开始没有来车
    res = 0;
    res += sem_init(&sem_north, 0, 0);
    res += sem_init(&sem_east, 0, 0);
    res += sem_init(&sem_south, 0, 0);
    res += sem_init(&sem_west, 0, 0);
    if (res != 0)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    /* 初始化线程 */
    res = 0;
    res += pthread_create(&thread_1, NULL, thread_south, NULL);
    res += pthread_create(&thread_2, NULL, thread_east, NULL);
    res += pthread_create(&thread_3, NULL, thread_north, NULL);
    res += pthread_create(&thread_4, NULL, thread_west, NULL);
    if (res != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    /*  */
    for (int i = 0; i < strlen(incoming_queue); ++i)
    {
        switch (incoming_queue[i])
        {
            case 'n':
                sem_post(&sem_north);
                break;
            case 'e':
                sem_post(&sem_east);
                break;
            case 's':
                sem_post(&sem_south);
                break;
            case 'w':
                sem_post(&sem_west);
                break;
        }   
    }
}

/* 定义线程 */
// 北向南车道
void *thread_north(void *arg)
{
    // 北向来车才会继续执行
    sem_wait(&sem_north);
    pthread_mutex_lock(&mutex_c);
    pthread_mutex_lock(&mutex_d);
    pthread_mutex_unlock(&mutex_c);
    pthread_mutex_unlock(&mutex_d);
}

// 东向西车道
void *thread_east(void *arg)
{
    // 东向来车才会继续执行
    sem_wait(&sem_east);
    pthread_mutex_lock(&mutex_b);
    pthread_mutex_lock(&mutex_c);
    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_c);
}

// 南向北车道
void *thread_south(void *arg)
{
    // 南向来车才会继续执行
    sem_wait(&sem_south);
    pthread_mutex_lock(&mutex_a);
    pthread_mutex_lock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
}

// 西向东车道
void *thread_west(void *arg)
{
    // 西向来车才会继续执行
    sem_wait(&sem_west);
    pthread_mutex_lock(&mutex_d);
    pthread_mutex_lock(&mutex_a);
    pthread_mutex_unlock(&mutex_d);
    pthread_mutex_unlock(&mutex_a);
}