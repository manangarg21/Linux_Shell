#include "headers.h"

pid_t pids[100];
char backs[100][4096];
int num = 0;
extern pinfo processes[4096];
extern int num_processes;

void background()
{
    int status;
    int i=0;
    while(i<num)
    {
        // for(int i=0;i<100;i++)
        // {
        //     printf("%s\n",backs[i]);
        // }
        pid_t pid = waitpid(pids[i], &status, WNOHANG);
        if (pid > 0)
        {
            // printf("%d\n", pids[i]);
            if (WIFEXITED(status))
            {
                printf("%s exited normally (%d)\n",backs[i],pids[i]);
            }
            else if(!WIFSIGNALED(status))
            {
                printf("%s exited abnormally (%d)\n",backs[i],pids[i]);
            }
            else
            {
                i++;
                continue;
            }
            
            // printf("%s\n", backs[i]);
            for (int j = i; j < num - 1; j++)
            {
                pids[j] = pids[j + 1];
                strcpy(backs[j], backs[j + 1]);
            }
            num--;

        }
        else if(pid==0)
        {
            
            i++;
        }

        else if(pid == -1 && errno==ECHILD)
        {
            break;
        }

        else
        {
            perror("wait");
            i++;
        }
    }
}

void bgtobg(char* input)
{
    populate_structs();
    // printf("%s\n",input);
    char*t = strtok(input," \n");
    // printf("%s\n",t);
    int id = atoi(t);
    // printf("%d\n",id);
    int flag=0;
    for(int i=0;i<num_processes;i++)
    {
        if(processes[i].pidval==id)
        {
            flag=1;
            break;
        }
    }

    if(flag==1)
    {
        int status;
        int res = kill(id,SIGCONT);
        if(res==-1)
        {
            perror("kill doesnt work");
        }
    }
    else
    {
        printf("No such process found\n");
    }
}