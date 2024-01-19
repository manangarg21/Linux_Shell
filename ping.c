#include "headers.h"

extern pinfo processes[4096];
extern int num_processes;
extern pid_t pid;
extern char dir1[4096];
extern char inputtt[4096];

pid_t fgpid;

void ctrlc()
{
    // printf("1\n");
    fgpid = pid;
    if (fgpid > 0)
    {
        printf("\n");
        kill(fgpid, SIGINT);
        pid = -1;
    }
    else
    {

        // printf("%s\n",dir1);
        putchar('\n');
        prompt(dir1);
        fflush(stdout);
    }
}

void ctrlz()
{
    populate_structs();
    if (pid > 0)
    {
        kill(pid, SIGTSTP);
        // printf("1\n");
        // inputtt[strlen(inputtt) - 1] = '\0';
        printf("\n");
        // Store the process information in the array of structs
        if (num_processes < 4096)
        {
            // printf("1\n");
            processes[num_processes].pidval = pid;
            // printf("1\n");
            processes[num_processes].cmdinfo = strdup(inputtt);
            // printf("1\n");
            processes[num_processes].state = 'T';
            num_processes++;
            pid = -1;
        }
    }
    else
    {
        putchar('\n');
        prompt(dir1);
        fflush(stdout);
    }
}

void pingcmd(int signum, int targetpid)
{
    populate_structs();
    signum %= 32;
    // printf("%d %d\n",signum,targetpid);
    int flag = 0, ii;
    for (ii = 0; ii < num_processes; ii++)
    {
        if (processes[ii].pidval == targetpid)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("No such process found\n");
        return;
    }
    
    int result = kill(targetpid, signum);
    if (result != 0)
    {
        perror("kill");
    }
    else
    {
        // processes[ii].state = 'K';
        printf("Sent signal %d to process with pid %d\n", signum, targetpid);
    }
}
