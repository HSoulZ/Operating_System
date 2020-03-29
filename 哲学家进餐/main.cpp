#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<process.h>

#define N 5                     //哲学家个数
#define LEFT(i) (N+i-1)%N       //左边哲学家编号
#define RIGHT(i) (i+1)%N        //右边哲学家编号
#define THINKING 0              //思考
#define HUNGRY 1                //饥饿
#define EATING 2                //吃饭

int state[N];                   //哲学家状态数组
HANDLE mutex;                   //状态访问互斥量
HANDLE s[N];                    //哲学家线程信号量A,刀叉
HANDLE thr_mutex;                  //线程同步信号量

//********按照PPT算法改善5写的***********

void think(int i);
void eat(int i);
void take_forks(int i);
void put_forks(int i);
void test(int i);
void print_state(void);

unsigned int __stdcall philosopher(void *ID)
{
    int i,n=3;
    i = *(int*)ID;
    ReleaseSemaphore(thr_mutex, 1, NULL);

    while(n--)
    {
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
    printf("---------------哲学家%d吃完了一日三餐\n",i);
    return 0;
}

void take_forks(int i)
{
    WaitForSingleObject(mutex, INFINITE);           //P(mutex)
    state[i] = HUNGRY;
    printf("哲学家%d饿了\n",i);
    test(i);
    ReleaseSemaphore(mutex, 1, NULL);               //V(mutex)
    WaitForSingleObject(s[i], INFINITE);            //P(s[i])
}

void put_forks(int i)
{
    WaitForSingleObject(mutex, INFINITE);  //P(mutex)
    state[i] = THINKING;
    test(LEFT(i));                         //检查左边哲学家是否可以就餐
    test(RIGHT(i));                        //检查右边哲学家是否可以就餐
    ReleaseSemaphore(mutex, 1, NULL);      //V(mutex)
}

void test(int i)
{
    if(state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING)
    {
        state[i] = EATING;
        ReleaseSemaphore(s[i], 1, NULL);    //V(s[i])
    }
}

void eat(int i)
{
    printf("哲学家%d在进餐\n", i);        //已在test()中将状态置为EATING
}

void think(int i)
{
    state[i]=THINKING;
    printf("哲学家%d在思考\n", i);
}

void print_state(void)
{
    int i;
    for(i = 0;i < N;i++)
    {
        printf("%d ", state[i]);
    }
    printf("\n");
}

int main()
{
    int i;
    HANDLE thread[N];
    mutex = CreateSemaphore(NULL, 1, 1, NULL);

    for(i = 0;i < N;i++)
        s[i] = CreateSemaphore(NULL, 0, 1, NULL);

    thr_mutex = CreateSemaphore(NULL, 0, 1, NULL);

    for(i = 0;i < N;i++)
    {
        thread[i] = (HANDLE)_beginthreadex(NULL, 0,philosopher, &i, 0, NULL);
        WaitForSingleObject(thr_mutex, INFINITE);
    }
    WaitForMultipleObjects(N, thread, TRUE, INFINITE);

    return 0;
}
