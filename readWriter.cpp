#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#include<vector>
#include<algorithm>
using namespace std;

//reader first init
sem_t mutex;
sem_t wmutex;//write mutex
int reader=0;
int writer=0;
int priority,n;

//writer first init
int readcount=0;
int writecount=0;
sem_t rmutex;//read mutex
sem_t mutex1;//mutex for readcount
sem_t mutex2;//mutex for writecount

struct people
{
    int pid;
    int start;
    int duration;
    char role;

    people(int pid,char role,int start,int duration)
    {
        this->pid = pid;
        this->role = role;
        this->start = start;
        this->duration = duration;
    }
};
bool cmp(const people&a, const people& b)
{
    return a.start<b.start;
}
vector<people> peoples;

//reader first
void* reader_thread(void* param)
{
    people p = *(people*)param;
    int id = p.pid;
    printf("reader %d waiting to read\n",id);
    sem_wait(&mutex);
    reader++;
    if(reader==1) {sem_wait(&wmutex);}
    sem_post(&mutex);
    printf("reader %d starts to read\n",id);
    //cout<<id<<" "<<peoples[id].duration<<endl;
    sleep(p.duration);//problem
    printf("reader %d ends reading\n",id);
    sem_wait(&mutex);
    reader--;
    if(reader==0) {sem_post(&wmutex);}
    sem_post(&mutex);
}

void* writer_thread(void * param)
{
    people p = *(people*)param;
    int id = p.pid;
    printf("writer %d waiting to write\n",id);
    sem_wait(&wmutex);
    printf("writer %d starts to write\n",id);
    sleep(p.duration);//problem
    printf("writer %d ends writing\n",id);
    sem_post(&wmutex);
}


//writer first
void* reader_thread1(void* param)
{
    people p = *(people*)param;
    int id = p.pid;
    printf("reader %d waiting to read\n",id);
    sem_wait(&mutex);
    sem_wait(&rmutex);
    sem_wait(&mutex1);

    readcount++;
    if(readcount==1)
    sem_wait(&wmutex);

    sem_post(&mutex1);
    sem_post(&rmutex);
    sem_post(&mutex);
    printf("reader %d starts to read\n",id);
    sleep(p.duration);//problem
    printf("reader %d ends reading\n",id);
    sem_wait(&mutex1);
    readcount--;
    if(readcount==0)
    sem_post(&wmutex);// let writer write
    sem_post(&mutex1);
}

void* writer_thread1(void *param)
{
    people p = *(people*)param;
    int id = p.pid;
    printf("writer %d waiting to write\n",id);
    sem_wait(&mutex2);

    writecount++;
    if(writecount==1)
    sem_wait(&rmutex);
    sem_post(&mutex2);

    sem_wait(&wmutex);
    sleep(p.duration);//problem
    printf("writer %d ends writing\n",id);
    sem_post(&wmutex);

    sem_wait(&mutex2);
    writecount--;
    if(writecount==0)
    sem_post(&rmutex);
    sem_post(&mutex2);
}
int main()
{
    cin>>priority>>n;
    for(int i=0;i<n;i++)
    {
        int id,start,duration;
        char role;
        cin>>id>>role>>start>>duration;
        peoples.push_back(people(id,role,start,duration));
    }

    sort(peoples.begin(),peoples.end(),cmp);
    // for(int i=0;i<n;i++)
    // {
    //     cout<<peoples[i].pid<<" "<<peoples[i].duration<<endl;
    // }
    if(priority==1)
    {
        sem_init(&mutex,0,1);
        sem_init(&wmutex,0,1);

        pthread_t pid[n];
        for(int i=0,j=0;i<n&&j<=peoples[n-1].start;j++)
        {
            if(peoples[i].start==j)
            {
                if(peoples[i].role=='R')
                {
                    pthread_create(&pid[i],NULL,reader_thread,(void*)&peoples[i]);
                }
                else 
                {
                    pthread_create(&pid[i],NULL,writer_thread,(void*)&peoples[i]);
                }
                i++;
                //cout<<"hello"<<endl;
            }
            sleep(1);
        }

        for(int i=0;i<n;i++)
        {
            pthread_join(pid[i],NULL);
        }
    }
    else
    {
        sem_init(&mutex1,0,1);
        sem_init(&mutex2,0,1);
        sem_init(&mutex,0,1);
        sem_init(&rmutex,0,1);
        sem_init(&wmutex,0,1);
        pthread_t pid[n];
        for(int i=0,j=0;i<n&&j<=peoples[n-1].start;j++)
        {
            if(peoples[i].start==j)
            {
                if(peoples[i].role=='R')
                {
                    pthread_create(&pid[i],NULL,reader_thread1,(void*)&peoples[i]);
                }
                else pthread_create(&pid[i],NULL,writer_thread1,(void*)&peoples[i]);
                i++;
                //cout<<"hello"<<endl;
            }
            sleep(1);
        }

        for(int i=0;i<n;i++)
        {
            pthread_join(pid[i],NULL);
        }
    }
}