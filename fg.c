#include "headers.h"

extern pid_t pids[100];
pid_t pid = -1;
extern int num;
extern int shellid;
extern pinfo processes[4096];
extern int num_processes;

int execfore(char *input, char **shell)
{
    time_t start, end;
    start = time(NULL);

    pid = fork();
    if (pid < 0)
    {
        perror("try again!");
        return -1;
    }
    else if (pid == 0)
    {
        setpgid(0, 0);
        // printf("1\n");
        int z = execvp("/bin/bash", shell);
        if (z == -1)
        {
            perror("Error: Command Does Not Exist!!");
        }
    }
    else
    {
        // printf("2\n");
        int status;
        waitpid(pid, &status, WUNTRACED);
        // printf("3\n");
        end = time(NULL);

        if (WIFEXITED(status))
        {
            // printf("4\n");
            int eta = (int)(end - start);
            char t[4096];
            sprintf(t, "%d", eta);
            if (eta > 2)
            {
                return eta;
            }
        }
    }
    return 0;
}

void bgtofg(char *input)
{
    populate_structs();
    char *t = strtok(input, " \n");
    // printf("%s\n",t);
    int id = atoi(t);
    // printf("%d\n",id);
    int flag = 0;
    for (int i = 0; i < num_processes; i++)
    {
        if (processes[i].pidval == id)
        {
            flag = 1;
            break;
        }
    }

    if (flag == 1)
    {
        int status;
        // tcgetattr(STDIN_FILENO,&settings);
        // shellid = getpid();
        pid = id;
        tcsetpgrp(shellid, id);
        kill(id, SIGCONT);

        pid_t res = waitpid(id, &status, WUNTRACED);
        tcsetpgrp(shellid, getpgrp());
        // tcsetattr(STDIN_FILENO,TCSADRAIN,&settings);
        if (res == -1)
        {
            perror("waitpid doesnt work");
        }
        else if (res == 0)
        {
            printf("No such process found\n");
        }
    }
    else
    {
        printf("No such process found\n");
    }
}
