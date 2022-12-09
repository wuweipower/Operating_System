#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<pthread.h>
using namespace std;

int share=0;

void* child(void*)
{
    while(1)
    {
        share--;
        cout<<"child:"<<share<<endl;
        sleep(1);
    }

}

int main()
{
    pthread_t t;
    pthread_create(&t,NULL,child,NULL);
    while (1)
    {
        share++;
        cout<<"parenet:"<<share<<endl;
        sleep(1);
    }
}
