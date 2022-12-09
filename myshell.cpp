#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<wait.h>
using namespace std;

const int NUM = 32;

string history[10]={""};
int history_index=0;

int main()
{   
    while (1)
    {
        string tips=getcwd(NULL,256);
        cout<<tips+"#:";

        char buf[1024]={0};
        fgets(buf,sizeof(buf)-1,stdin);
        buf[strlen(buf)-1]=0;

        char *argv[NUM];
        argv[0]=strtok(buf," ");//字符串分割
        int i=0;
        while(argv[i]!=NULL)
        {
            i++;
            argv[i]= strtok(NULL," ");
        }
        //printf("%s\n",argv[1]);
        for(int i =0;i<sizeof(argv);i++)
        {
            string temp;
            if(argv[i]!=NULL)
            {
                temp = argv[i];
                history[history_index] +=temp+" ";
            }   
            else break;
        }
        history_index = (history_index+1)%10;
        bool flag= false;// to distinguish internal and external
        for(int i =0;i<sizeof(argv);i++)
        {
            string temp;
            if(argv[i]!=NULL)
            temp = argv[i];
            else
            break;
            if(temp=="cd")
            {
                chdir(argv[i+1]);
                flag=true;
                break;
            }
            if(temp=="exit")
            {
                cout<<"ok"<<endl;
                return 0;
            }
            if(temp=="history")
            {
                for(int i =0;i<10;i++)
                {
                    if(history[i]!="")
                    cout<<history[i]<<endl;
                }
                flag = true;
            }          
        }
        if(!flag)
        {
            pid_t id = fork();
            if(id==0)
            {
                execvp(argv[0],argv);
            }
            else
            {
                int status=0;
                waitpid(id,&status,0);       
            }
        }
    }

}