#include "headers.h"

void pipe1(char *input, char **shell, char *dir, char *prev_dir)
{
    int count = 0;
    char *a;
    char *b;
    char *copy = (char *)malloc(4096);
    strcpy(copy, input);

    a = strtok(input, "|");
    while (a != NULL)
    {
        a = strtok(NULL, "|");
        count++;
    }

    char *pipe_arr[count];
    int cnt = 0;
    b = strtok(copy, "|\n");
    while (b != NULL)
    {
        pipe_arr[cnt] = b;
        cnt++;
        b = strtok(NULL, "|\n");
    }
    // printf("%d %d\n",count,cnt);
    if(cnt != count)
    {
        printf("Invalid pipe command, Need more arguments");
    }

    // for(int i=0;i<count;i++)
    // {
    //     printf("%s\n",pipe_arr[i]);
    // }
    int file = 0;

    for (int i = 0; i < cnt; i++)
    {
        int flags[2];
        int p = pipe(flags);
        if (p == -1)
        {
            perror("pipe failed");
            exit(1);
        }

        pid_t pid2 = fork();

        if (pid2 == -1)
        {
            perror("fork failed");
            exit(1);
        }
        else if (pid2 == 0)
        {
            // Child process
            close(flags[0]);

            if (file != 0)
            {
                dup2(file, STDIN_FILENO);
                close(file);
            }

            if (i < cnt - 1)
            {
                dup2(flags[1], STDOUT_FILENO);
            }

            close(flags[1]);

            char *x;
            char *t = strtok_r(pipe_arr[i], ";", &x);
            // char* cd=strtok(input," \t\n");
            // char *shell[] = {"sh", "-c", NULL, NULL};
            shell[2] = strdup(t);

            while (t != NULL)
            {
                if (strstr(t, "neonate") != NULL)
                {
                    neonate(t);
                }
                else if (strstr(t, "fg") != NULL)
                {
                    // printf("1\n");
                    t += 2;
                    bgtofg(t);
                }
                else if (strstr(t, "bg") != NULL)
                {
                    t += 2;
                    bgtobg(t);
                }
                else if (strstr(t, "iMan") != NULL)
                {
                    t += 4;
                    manpage(t);
                }
                else if (strstr(t, "activities") != NULL)
                {
                    // printf("1\n");
                    activity();
                    // printf("1\n");
                }
                else if (strstr(t, "<") != NULL)
                {
                    read1(t, shell, dir, prev_dir);
                    if (strstr(t, ">>") != NULL)
                    {
                        write1(t, 1, shell, dir, prev_dir);
                    }
                    else if (strstr(t, ">") != NULL)
                    {
                        write1(t, 0, shell, dir, prev_dir);
                    }
                }

                else if (strstr(t, ">>") != NULL)
                {
                    write1(t, 1, shell, dir, prev_dir);
                    if (strstr(t, "<") != NULL)
                    {
                        read1(t, shell, dir, prev_dir);
                    }
                }

                else if (strstr(t, ">") != NULL)
                {
                    write1(t, 0, shell, dir, prev_dir);
                }

                else if (strstr(t, "warp") != NULL)
                {
                    warp_dir(t, dir, prev_dir);
                }

                else if (strstr(t, "peek") != NULL)
                {
                    peek_terminal(t, dir);
                }
                else if (strstr(t, "proclore") != NULL)
                {
                    getinfo(t);
                }

                else if (strstr(t, "seek") != NULL)
                {
                    seek_execute(t, dir);
                }
                else
                {
                    shell[2] = strdup(t);
                    int pid1 = fork();
                    if (pid1 == 0)
                    {
                        // setpgid(0, 0);
                        execvp("/bin/bash", shell);
                    }
                    else
                    {
                        wait(NULL);
                    }
                }
                t = strtok_r(NULL, ";", &x);
            }
            exit(1);
        }
        else
        {
            if (file != 0)
            {
                close(file);
            }

            close(flags[1]);
            file = flags[0];
            wait(NULL);
        }
    }
    free(copy);
}