#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/stat.h>
#include<iostream>
#include<string>
#include<stdio.h>
#include<vector>
#include<regex>
#include<ctime>
#include<time.h>
using namespace std;

void getAllFiles(string dirname,vector<string>& files)
{
    DIR* dir;
    struct dirent* ptr;
    dir = opendir(dirname.c_str());
    while((ptr = readdir(dir))!=NULL)
    {
        if(ptr->d_type==DT_REG)
        {
            files.push_back(dirname+"/"+ptr->d_name);
        }
        else if(ptr->d_type==DT_DIR &&strcmp(ptr->d_name,".")!=0&&strcmp(ptr->d_name,"..")!=0)
        {
            getAllFiles(dirname+"/"+ptr->d_name,files);
        }
    }
    closedir(dir);
}
int main()
{
    DIR* dir;
    struct dirent* ptr;
    int index=0;
    vector<string> files;
    char ch;
    string cmd="";
    while ((ch=getchar())!='\n')
    {
        cmd+=ch;
        index++;
    }
    if(cmd.substr(0,6)!="myfind")
    {
        cout<<"Command error, the correct command is myfind"<<endl;
        exit(0);
    }
    //get filename
    int path_s = 7;
    int path_e = cmd.find("-")<cmd.length() ? cmd.find("-")-1:cmd.length();
    string dirname = cmd.substr(path_s,path_e-path_s);

    getAllFiles(dirname,files);
///////////////////////////////////////////
    string name = "";
    int file_index = cmd.find("-name");
    if(file_index<cmd.length()&& file_index!=-1)
    {
        int file_start = file_index+6;
        for(int i=file_start;i<cmd.length();i++)
        {
            if(cmd[i]==' ') break;
            if(cmd[i]=='?'||cmd[i]=='*')
            name+='.';

            name+=cmd[i];
        }
    }

    regex r(dirname+".*?"+name);
    vector<string> filter_files;
    int time_index = cmd.find("-mtime");

    if(time_index!=-1)//有-mtime 参数
    {
        for(int i =0;i<files.size();i++)
        {
            if(regex_match(files[i],r))
            {
                filter_files.push_back(files[i]);              
            }
        }
    }
    
    else
    {
        for(int i =0;i<files.size();i++)
        {
            if(regex_match(files[i],r))
            {
                cout<<files[i]<<endl;
            }
        }
    }

//////////////////////////////////////////////
    string days="";
    if(time_index < cmd.length() && time_index!=-1)
    {
        int time_start = time_index+7;
        for(int i =time_start;i<cmd.length();i++)
        {
            if(cmd[i]!=' ' && cmd[i]!='\n')
            {
                days+=cmd[i];
            }
        }
    }
    
    int daybefore=-1;
    if(days!="")
    {
        daybefore = stoi(days);
    }

    time_t now;
    time(&now);
    string now_day_str = ctime(&now);
    int now_day = stoi(now_day_str.substr(8,2));

    if(daybefore!=-1)
    {
        //先从ctime获取的时间里的天数进行判断,因为有年月的差别，如果小于就使用时间的秒数来判断
        for(int i=0;i<filter_files.size();i++)
        {
            struct stat f_info;
            int res = stat(filter_files[i].c_str(),&f_info);
            long time_len = daybefore*24*60*60;
            
            if(res==0)
            {
                string t = ctime(&f_info.st_mtime);
                int num =stoi(t.substr(8,2));
                //cout<<"num: "<<num<<endl;
                if(now_day-num>=daybefore)
                {
                    cout<<filter_files[i];
                    cout<<" "<<t;
                }
                else if(now-f_info.st_mtime>=time_len)
                {
                    cout<<filter_files[i];
                    cout<<" "<<t;
                }
                
            }
            else
            {
                cout<<"error"<<endl;
            }
        }      
    }
}