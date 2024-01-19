#include "headers.h"

void read1(char *input, char **shell, char *dir, char *prev_dir)
{
    char *a;
    char *b;
    // printf("%s\n",input);
    a = strtok(input, "<");
    b = strtok(NULL, " \t\n");
    // printf("%s\n",a);
    // printf("%s\n",b);
    int file = open(b, O_RDONLY);
    if (file < 0)
    {
        perror("No such file found!!");
    }
    else
    {
        // int pid = fork();

        int read = dup(STDIN_FILENO);
        dup2(file, STDIN_FILENO);
        close(file);
        // if (pid == 0)
        // {

        // printf("1\n");
        char *x;
        char *t = strtok_r(a, ";", &x);
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
        dup2(read, STDIN_FILENO);

        // }
        // else
        // {
        //     wait(NULL);
        // }
    }
}

void write1(char *input, int flag, char **shell, char *dir, char *prev_dir)
{
    char *a;
    char *b;
    // char *c;

    int final_flags;
    if (flag == 1)
    {
        a = strtok(input, ">");
        b = strtok(NULL, " >\t\n");
        // printf("%s\n",a);
        // b = strtok(NULL, " \t\n");
        final_flags = O_WRONLY | O_CREAT | O_APPEND;
    }
    else
    {
        a = strtok(input, ">");
        b = strtok(NULL, " \t\n");
        final_flags = O_WRONLY | O_CREAT | O_TRUNC;
    }
    // printf("%s\n",c);
    // printf("%s\n",b);
    int file = open(b, final_flags, 0644);
    // printf("%d\n",file);
    int read = dup(STDOUT_FILENO);

    dup2(file, STDOUT_FILENO);
    // printf("%d\n", STDOUT_FILENO);

    close(file);
    // if (pid == 0)
    // {

    // printf("1\n");
    char *x;
    char *t = strtok_r(a, ";", &x);
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
    dup2(read, STDOUT_FILENO);
}