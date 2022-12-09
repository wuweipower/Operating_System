#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<pthread.h>

#include<sys/syscall.h>
using namespace std;
void printTime(int id)
{
   time_t t;
   struct tm* lt;
   time(&t);
   lt=localtime(&t);
   printf("The %d is taking: %d %d %d %d:%d:%d\n",id,lt->tm_year+1900,lt->tm_mon+1,lt->tm_mday,
   lt->tm_hour,lt->tm_min,lt->tm_sec
   );
}

void* child(void*)
{
    while (1)
    {
        printTime(syscall(SYS_gettid));
        sleep(1);
    }
    return NULL;
}
int main()
{
    //printf("%s", "abcde");
    pthread_t t;
    pthread_create(&t,NULL,child,NULL);
    while(1)
    {
        printTime(syscall(SYS_gettid));
        sleep(1);
    }
}