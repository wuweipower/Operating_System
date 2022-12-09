#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

void parent()
{  
    char* const* args={NULL};
    char* const* enev={NULL};
    execve("task1.out",args,enev);
    //cout<<pid<<endl;
}
void printTime()
{
   time_t t;
   struct tm* lt;
   time(&t);
   lt=localtime(&t);
   printf("The parent is taking: %d %d %d %d:%d:%d\n",lt->tm_year+1900,lt->tm_mon+1,lt->tm_mday,
   lt->tm_hour,lt->tm_min,lt->tm_sec
   );
}
void printParent()
{
    while(1)
    {
        printTime();
        sleep(1);
    }
}
int main()
{
    pid_t pid;
    pid=fork();
    if(pid!=0)
    {
        parent();
        cout<<1<<endl;
    }
    else
    printParent();
}