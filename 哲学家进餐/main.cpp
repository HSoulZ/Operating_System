#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<process.h>

#define N 5                     //��ѧ�Ҹ���
#define LEFT(i) (N+i-1)%N       //�����ѧ�ұ��
#define RIGHT(i) (i+1)%N        //�ұ���ѧ�ұ��
#define THINKING 0              //˼��
#define HUNGRY 1                //����
#define EATING 2                //�Է�

int state[N];                   //��ѧ��״̬����
HANDLE mutex;                   //״̬���ʻ�����
HANDLE s[N];                    //��ѧ���߳��ź���A,����
HANDLE thr_mutex;                  //�߳�ͬ���ź���

//********����PPT�㷨����5д��***********

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
    printf("---------------��ѧ��%d������һ������\n",i);
    return 0;
}

void take_forks(int i)
{
    WaitForSingleObject(mutex, INFINITE);           //P(mutex)
    state[i] = HUNGRY;
    printf("��ѧ��%d����\n",i);
    test(i);
    ReleaseSemaphore(mutex, 1, NULL);               //V(mutex)
    WaitForSingleObject(s[i], INFINITE);            //P(s[i])
}

void put_forks(int i)
{
    WaitForSingleObject(mutex, INFINITE);  //P(mutex)
    state[i] = THINKING;
    test(LEFT(i));                         //��������ѧ���Ƿ���ԾͲ�
    test(RIGHT(i));                        //����ұ���ѧ���Ƿ���ԾͲ�
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
    printf("��ѧ��%d�ڽ���\n", i);        //����test()�н�״̬��ΪEATING
}

void think(int i)
{
    state[i]=THINKING;
    printf("��ѧ��%d��˼��\n", i);
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
