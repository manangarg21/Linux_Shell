#include "headers.h"

int y;
char str_global[4096];
char dir1[4096];
char inputtt[4096];
extern pid_t pids[100];
extern char backs[100][4096];
extern int num;
extern char coms[4096];
extern char coms2[4096];
extern char arr[15][4096];
extern int count;
extern int ec;
extern char es[4096];
int shellid;

int main()
{
    shellid = getpid();
    char *dir = (char *)malloc(4096);
    char *prev_dir = (char *)malloc(4096);
    char *cp = (char *)malloc(4096);

    getcwd(dir, 4096);
    strcpy(cp, dir);
    strcpy(dir1, dir);
    getcwd(prev_dir, 4096);
    // printf("%s\n",cp);
    while (1)
    // printf("%s\n",dir);
    // Keep accepting commands
    {
        // printf("1\n");
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(dir);
        char input[4096];
        char *copy = (char *)malloc(4096);
        input[0] = '\0';
        signal(SIGINT, ctrlc);
        signal(SIGTSTP, ctrlz);

        fgets(input, 4096, stdin);
        // printf("%s\n",input);
        strcpy(copy, input);
        // printf("%s\n",copy);

        copy[strcspn(copy, "\n")] = '\0';
        //  printf("%s\n",copy);
        strcpy(inputtt, copy);
        if (strcmp("copy", "\n") == 0)
        {
            continue;
        }
        int count = 0;

        if (feof(stdin)) // ctrl+D
        {
            printf("\n");
            break;
        }

        if (strstr(copy, "pastevents") == NULL)
        {
            append(copy, cp);
        }

        if (strcmp(copy, "exit") == 0)
        {
            append(copy, cp);
            break;
        }

        else
        {
            char *x;
            char *t = strtok_r(input, ";", &x);
            // char* cd=strtok(input," \t\n");
            // char **shell = (char **)malloc(sizeof(char *) * 4);
            // for (int i = 0; i < 4; i++)
            // {
            //     char *shell = (char *)malloc(4096);
            // }
            char *shell[] = {"sh", "-c", NULL, NULL};
            // printf("z\n");
            // strcpy(shell[0], "sh");
            // strcpy(shell[1], "-c");
            // printf("1\n");
            // shell[2] = NULL;
            // shell[3] = NULL;
            y = 0;
            while (t != NULL)
            {
                shell[2] = strdup(t);

                // char* io1=strstr(t,"<");
                // char* io2=strstr(t,"<<");
                // char* io3=strstr(t,">");
                // if(strstr(t,"iMan")!=NULL)
                // {

                // }
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
                else if (strstr(t, "ping") != NULL)
                {
                    int targetpid, signum;
                    if (sscanf(copy, "ping %d %d", &targetpid, &signum) == 2)
                    {
                        pingcmd(signum, targetpid);
                    }
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
                else if (strstr(t, "|") != NULL)
                {
                    // printf("1\n");
                    pipe1(t, shell, cp, prev_dir);
                }
                else if (strstr(t, "<") != NULL)
                {
                    // printf("1\n");
                    // strcpy(io1,a);
                    // read1(t, shell);
                    if (strstr(t, ">>") != NULL)
                    {
                        // write1(t, 1, shell);
                        write1(t, 1, shell, cp, prev_dir);
                        // execfore(t, shell);
                    }
                    else if (strstr(t, ">") != NULL)
                    {
                        // write1(t, 0, shell);
                        write1(t, 0, shell, cp, prev_dir);
                        // execfore(t, shell);
                    }
                    else
                    {
                        read1(t, shell, cp, prev_dir);
                    }
                    // execfore(a, shell);
                }

                else if (strstr(t, ">>") != NULL)
                {
                    // write1(t, 1, shell);
                    if (strstr(t, "<") != NULL)
                    {
                        // strcpy(io1,a);
                        // printf("2\n");
                        // read1(t, shell);
                        read1(t, shell, cp, prev_dir);
                    }
                    else
                    {
                        write1(t, 1, shell, cp, prev_dir);
                    }

                    // execfore(t, shell);
                }

                else if (strstr(t, ">") != NULL)
                {
                    // write1(t, 0, shell);
                    if (strstr(t, "<") != NULL)
                    {
                        // strcpy(io1,a);
                        // printf("2\n");
                        // read1(t, shell);
                        read1(t, shell, cp, prev_dir);
                    }
                    else
                    {
                        write1(t, 0, shell, cp, prev_dir);
                    }
                    // execfore(t, shell);
                }

                else if (strstr(t, "&") != NULL)
                {
                    int c1 = 0;
                    for (int i = 0; t[i] != '\0'; i++)
                    {
                        if (t[i] == '&')
                        {
                            c1++;
                        }
                    }
                    // printf("%d\n",c1);
                    // char *x1;
                    char *t1 = strtok(t, "&");
                    while (t1 != NULL && c1 > 0)
                    {
                        pid_t bg_pid = fork();
                        if (bg_pid == 0)
                        {
                            char *shell1[] = {"sh", "-c", NULL, NULL};
                            shell1[2] = t1;
                            setpgid(0, 0);
                            // printf("%s\n",t1);
                            execvp("/bin/bash", shell1);

                            exit(0);
                        }
                        else if (bg_pid > 0)
                        {
                            // Parent process
                            printf("%d\n", bg_pid);
                            pids[num] = bg_pid;
                            strcpy(backs[num], t1);
                            num++;
                        }
                        else
                        {
                            perror("Fork failed");
                        }
                        t1 = strtok(NULL, "&\n");
                        c1--;
                    }
                    if (t1 != NULL)
                    {
                        y = execfore(t1, shell);
                    }
                }
                // printf("%s\n",t);
                else if (strstr(t, "warp") != NULL)
                {
                    warp_dir(t, dir, prev_dir);
                }

                else if (strstr(t, "peek") != NULL)
                {
                    // if (strstr(t, "-l") != NULL || strstr(t, "-al") != NULL || strstr(t, "-la") != NULL)
                    // {
                    //     printf("total %lld\n", total);
                    // }
                    peek_terminal(t, dir);
                }

                else if (strstr(t, "proclore") != NULL)
                {
                    getinfo(t);
                }

                else if (strstr(t, "seek") != NULL)
                {
                    seek_execute(t, cp);
                }

                else if (strstr(t, "pastevents") != NULL)
                {
                    printhistory(t, cp, prev_dir);
                    // for (int i = 0; i < count; i++)
                    // {
                    //     printf("%s\n",arr[i]);
                    // }
                }

                else
                {
                    // printf("x\n");
                    strcpy(str_global, t);
                    y = execfore(t, shell);
                }
                t = strtok_r(NULL, ";\n", &x);
            }
            background();
        }
        free(copy);
    }
    free(dir);
    free(prev_dir);
    free(cp);

    return 0;
}
