#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#define MAX 100         //�ѣ�����RR�㷨�У���ž�������

//*******������̿�д����******


enum State{ready,run,block,finish};    //���������С����������
int n=0;
int time=0;
int sum=0;


typedef struct pcb
{
    int ID;           //&���̱�ʶ��
    State STATE;      //&����״̬
    int arr_time;     //����ʱ��
    int ser_time;     //&����ʱ��(��Ŀ������ռ�õ�CPUʱ��)
    int STARTBLOCK;   //&����������STARTBLOCK��ʱ��Ƭ�󽫽�������״̬
    int BLOCKTIME;    //&���������ٵȴ�BLOCKTIME��ʱ��Ƭ��ת���ɾ���״̬
    int run_time;     //&������ʱ��
    int star_time;    //��ʼʱ��
    int end_time;     //���ʱ��
    int left_time;    //ʣ�����ʱ��
    struct pcb *NEXT; //&����ָ��
}PCB,*PCBLink;


void hub(int a,PCBLink h);
void FCFS(PCBLink h);
void SJF(PCBLink h);
void SRT(PCBLink h);
void RR(PCBLink h);

//��������
PCBLink input()
{
    int i;
    PCBLink h,p,q;
    h=(PCB*)malloc(sizeof(PCB));
    h->NEXT=NULL;
    q=h;

    printf("������Ŀ��");
    scanf("%d",&n);

    for(i=0;i<n;i++)
    {
        p=(PCBLink)malloc(sizeof(PCB));
        printf("���������%d�ĵ���ʱ�䡢����ʱ��:\n",i+1);
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

//�㷨ѡ��
void choose(PCBLink h)
{
    int a;
    printf("�㷨ѡ��\n");
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

//���ܷ���
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

//�����ȷ���FCFS
void FCFS(PCBLink h)
{
    int i,first=0;        //first������¼�����еĵ�x�����̣���Ϊ��һ�����̣���������״̬
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
                    first=0;           //��ɺ���firstΪ0
                }
            }
            p=p->NEXT;
        }
        printf("ʱ��Ƭ%d:\n",time);
        printf("����ID\t״̬\t����ʱ��\t����ʱ��\t������ʱ��\t������ʱ��\n");
        q=h->NEXT;
        while(q)
        {
            printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n",q->ID,q->STATE,q->arr_time,q->ser_time,q->run_time,q->BLOCKTIME);
            q=q->NEXT;
        }
    }
}

//����ҵ����SJF
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
                    p=h;            //������ɺ�λ�������������ҵ
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
        printf("ʱ��Ƭ%d:\n",time);
        printf("����ID\t״̬\t����ʱ��\t����ʱ��\t������ʱ��\t������ʱ��\n");
        s=h->NEXT;
        while(s)
        {
            printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\n",s->ID,s->STATE,s->arr_time,s->ser_time,s->run_time,s->BLOCKTIME);
            s=s->NEXT;
        }
    }
}

//���ʣ��ʱ��SRT
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
                    p=h;            //������ɺ�λ��������ʣ�������ҵ
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
                while(r)        //���㻹�ж�ý�������̬
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
        printf("ʱ��Ƭ%d:\n",time);
        printf("����ID\t״̬\t����ʱ��\t����ʱ��\t������ʱ��\t����ʱ��\t������ʱ��\n");
        s=h->NEXT;
        while(s)
        {
            printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",s->ID,s->STATE,s->arr_time,s->ser_time,s->run_time,s->STARTBLOCK,s->BLOCKTIME);
            s=s->NEXT;
        }
    }
}

//��תRR
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
            m=NULL;             //m��λ
        }

        if(s.r!=s.f)
        {
            t=s.data[s.f];
            s.f=(s.f+1)%MAX;
            t->STATE=run;
        }
        else if(s.r==s.f && time!= sum-1)
            exit(0);

        printf("ʱ��Ƭ%d:\n",time);
        printf("����ID\t״̬\t����ʱ��\t����ʱ��\t������ʱ��\t������ʱ��\n");
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
