#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#define MAX 100         //堆，用在RR算法中，存放就绪队列

//*******面向过程快写哭了******


enum State{ready,run,block,finish};    //就绪、运行、阻塞、完成
int n=0;
int time=0;
int sum=0;


typedef struct pcb
{
    int ID;           //&进程标识数
    State STATE;      //&进程状态
    int arr_time;     //到达时间
    int ser_time;     //&服务时间(题目给出的占用的CPU时间)
    int STARTBLOCK;   //&进程再运行STARTBLOCK个时间片后将进入阻塞状态
    int BLOCKTIME;    //&阻塞进程再等待BLOCKTIME个时间片后将转换成就绪状态
    int run_time;     //&已运行时间
    int star_time;    //开始时间
    int end_time;     //完成时间
    int left_time;    //剩余服务时间
    struct pcb *NEXT; //&队列指针
}PCB,*PCBLink;


void hub(int a,PCBLink h);
void FCFS(PCBLink h);
void SJF(PCBLink h);
void SRT(PCBLink h);
void RR(PCBLink h);

//进程输入
PCBLink input()
{
    int i;
    PCBLink h,p,q;
    h=(PCB*)malloc(sizeof(PCB));
    h->NEXT=NULL;
    q=h;

    printf("进程数目：");
    scanf("%d",&n);

    for(i=0;i<n;i++)
    {
        p=(PCBLink)malloc(sizeof(PCB));
        printf("请输入进程%d的到达时间、服务时间:\n",i+1);
        scanf("%d %d",&p->arr_time,&p->ser_time);
        p->ID=i+1;
        p->STATE=block;
        p->run_time=0;
        p->left_time=p->ser_time;
        p->NEXT=NULL;

        q->NEXT=p;
        q=p;
    }
    return h;
}

//算法选择
void choose(PCBLink h)
{
    int a;
    printf("算法选择\n");
    printf("1.FCFS\n");
    printf("2.SJF\n");
    printf("3.SRT\n");
    printf("4.RR\n");
    scanf("%d",&a);
    if(!(a<=4 && a>=1))
        choose(h);
    else
        hub(a,h);
}

//功能分流
void hub(int a,PCBLink h)
{
    switch(a)
    {
        case 1:
            FCFS(h);
            break;
        case 2:
            SJF(h);
            break;
        case 3:
            SRT(h);
            break;
        case 4:
            RR(h);
            break;
        default:
            break;
    }
}

//先来先服务FCFS
void FCFS(PCBLink h)
{
    int i,first=0;        //first用来记录就绪中的第x个进程，若为第一个进程，进入运行状态
    PCBLink p,q;

    for(time=0;time<sum;time++)
    {
        p=h->NEXT;
        while(p)
        {
            p->BLOCKTIME = (p->arr_time-time)>=0? (p->arr_time-time):0;
            if((p->BLOCKTIME==0)&&(p->STATE==block))
            {
                first++;
                if(first==1)
                    {p->STATE = run;p->run_time=0;}
                else
                    p->STATE = ready;
            }
            else if((p->BLOCKTIME==0)&&(p->STATE==ready))
            {
                first++;
                if(first==1)
                    {p->STATE = run;p->run_time=0;}
            }
            else if(p->STATE==run)
            {
                p->run_time++;
                if(p->run_time==p->ser_time)
                {
                    p->STATE = finish;
                    first=0;           //完成后置first为0
                }
            }
            p=p->NEXT;
        }
        printf("时间片%d:\n",time);
        printf("进程ID\t状态\t到达时间\t服务时间\t已运行时间\t被阻塞时间\n");
        q=h->NEXT;
        while(q)
        {
            printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n",q->ID,q->STATE,q->arr_time,q->ser_time,q->run_time,q->BLOCKTIME);
            q=q->NEXT;
        }
    }
}

//短作业优先SJF
void SJF(PCBLink h)
{
    int mini,first=0;
    PCBLink p,q,s;

    for(time=0;time<sum;time++)
    {
        p=h->NEXT;
        while(p)
        {
            p->BLOCKTIME = (p->arr_time-time)>=0? (p->arr_time-time):0;
            if((p->BLOCKTIME==0)&&(p->STATE==block))
            {
                p->STATE = ready;
                first++;
                if(first==1)
                    mini=p->ser_time;
                else if(p->ser_time < mini)
                    mini=p->ser_time;
            }
            else if((p->BLOCKTIME==0)&&(p->STATE==ready))
            {
                first++;
                if(first==1)
                    mini=p->ser_time;
                else if(p->ser_time < mini)
                    mini=p->ser_time;
            }
            else if(p->STATE==run)
            {
                p->run_time++;
                if(p->run_time==p->ser_time)
                {
                    p->STATE = finish;
                    first = 0;
                    p=h;            //进程完成后复位，重新找最短作业
                }
            }
            p = p->NEXT;
        }
        q=h->NEXT;
        while(q)
        {
            if((q->ser_time==mini)&&(q->STATE==ready))
            {
                q->STATE = run;
                q->run_time = 0;
                break;
            }
            else q=q->NEXT;
        }
        printf("时间片%d:\n",time);
        printf("进程ID\t状态\t到达时间\t服务时间\t已运行时间\t被阻塞时间\n");
        s=h->NEXT;
        while(s)
        {
            printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n",s->ID,s->STATE,s->arr_time,s->ser_time,s->run_time,s->BLOCKTIME);
            s=s->NEXT;
        }
    }
}

//最短剩余时间SRT
void SRT(PCBLink h)
{
    int mini,first=0;
    PCBLink p,q,s,r;

    for(time=0;time<sum;time++)
    {
        p=h->NEXT;
        while(p)
        {
            p->BLOCKTIME = (p->arr_time-time)>=0? (p->arr_time-time):0;
            if((p->BLOCKTIME==0)&&(p->STATE==block))
            {
                p->STATE = ready;
                first++;
                if(first==1)
                    mini=p->left_time;
                else if(p->left_time < mini)
                    mini=p->left_time;
            }
            else if((p->BLOCKTIME==0)&&(p->STATE==ready))
            {
                first++;
                if(first==1)
                    mini=p->left_time;
                else if(p->left_time < mini)
                    mini=p->left_time;
            }
            else if(p->STATE==run)
            {
                p->run_time++;
                p->left_time--;
                mini--;
                p->STARTBLOCK=(p->STARTBLOCK>0)?(p->STARTBLOCK-1):0;
                if(p->STARTBLOCK==0)
                {
                    p->STATE=block;
                }
                if(mini==0)
                {
                    p->STATE = finish;
                    first = 0;
                    p=h;            //进程完成后复位，重新找剩余最短作业
                }
            }
            p = p->NEXT;
        }
        q=h->NEXT;
        while(q)
        {
            if((q->left_time==mini)&&(q->STATE==ready))
            {
                q->STATE = run;
                r=q->NEXT;
                if(!r)
                    break;
                while(r)        //计算还有多久进入阻塞态
                {
                    if((r->left_time+r->BLOCKTIME)<q->left_time)
                        {q->STARTBLOCK = r->BLOCKTIME;break;}
                    else q->STARTBLOCK = 0;
                    r = r->NEXT;
                }
                break;
            }
            else if((q->left_time==mini)&&(q->STATE==block))
                q->STATE = run;

            else q=q->NEXT;
        }
        printf("时间片%d:\n",time);
        printf("进程ID\t状态\t到达时间\t服务时间\t已运行时间\t阻塞时间\t被阻塞时间\n");
        s=h->NEXT;
        while(s)
        {
            printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",s->ID,s->STATE,s->arr_time,s->ser_time,s->run_time,s->STARTBLOCK,s->BLOCKTIME);
            s=s->NEXT;
        }
    }
}

//轮转RR
void RR(PCBLink h)
{
        typedef struct
    {
        PCBLink data[MAX];
        int f,r;
    }SeqStack;

    SeqStack s;
    PCBLink p,q,t,m=NULL;
    s.f=s.r=0;

    for(time = 0;time<sum;time++)
    {
        p=h->NEXT;
        while(p)
        {
            p->BLOCKTIME = (p->arr_time-time)>=0? (p->arr_time-time):0;
            if((p->BLOCKTIME==0)&&(p->STATE==block))
            {
                p->STATE = ready;
                if((s.r+1)%MAX==s.f)
                    exit(0);
                s.data[s.r]=p;
                s.r=(s.r+1)%MAX;
            }
            else if(p->STATE==run)
            {
                p->run_time++;
                p->left_time--;
                if(p->left_time==0)
                    p->STATE = finish;
                else
                {
                    p->STATE = ready;
                    m=p;
                }
            }
            p=p->NEXT;
        }
        if(m)
        {
            if((s.r+1)%MAX==s.f)
                exit(0);
            s.data[s.r]=m;
            s.r=(s.r+1)%MAX;
            m=NULL;             //m复位
        }

        if(s.r!=s.f)
        {
            t=s.data[s.f];
            s.f=(s.f+1)%MAX;
            t->STATE=run;
        }
        else if(s.r==s.f && time!= sum-1)
            exit(0);

        printf("时间片%d:\n",time);
        printf("进程ID\t状态\t到达时间\t服务时间\t已运行时间\t被阻塞时间\n");
        q=h->NEXT;
        while(q)
        {
            printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n",q->ID,q->STATE,q->arr_time,q->ser_time,q->run_time,q->BLOCKTIME);
            q=q->NEXT;
        }
    }
}

int main()
{
    PCBLink h,p;
    h = input();
    p=h->NEXT;
    while(p)
    {
        sum+=p->ser_time;
        p=p->NEXT;
    }
    sum+=h->NEXT->arr_time+1;
    choose(h);

    return 0;
}
