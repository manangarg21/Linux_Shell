#include "headers.h"

char coms[4096];
char coms2[4096];
char arr[15][4096];
int count = 0;

void append(char *input, char *dir)
{
    int count1 = 0;

    strcpy(coms, dir);
    strcpy(coms2, dir);
    strcat(coms, "/history.txt");
    strcat(coms2, "/temp.txt");

    if (access(coms, F_OK) == 0)
    {
        FILE *check = fopen(coms, "r");
        if (check != NULL)
        {
            char *line = NULL;
            int read;
            size_t len = 0;
            // char i;
            // for (i = getc(check); i != EOF; i = getc(check))
            // {
            //     if (i == '\n')
            //     {
            //         count1++;
            //     }
            // }
            while ((read = getline(&line, &len, check)) != -1)
            {
                if (line[read - 1] == '\n')
                {
                    line[read - 1] = '\0';
                }
                strcpy(arr[count1], line);
                count1++;
            }
        }
        fclose(check);
    }

    FILE *file = fopen(coms, "a");
    if (file != NULL)
    {
        // printf("%d\n", count1);
        if (count1 > 0 && strcmp(arr[count1 - 1], input) == 0)
        {
        }

        else
        {
            // printf("1\n");
            if (count1 >= 15)
            {
                FILE *tempFile = fopen(coms2, "w");
                if (tempFile != NULL)
                {
                    for (int i = 1; i < 15; i++)
                    {
                        strcpy(arr[i - 1], arr[i]);
                    }
                    strcpy(arr[14], input);
                    for (int i = 0; i < 15; i++)
                    {
                        fprintf(tempFile, "%s\n", arr[i]);
                    }

                    fclose(tempFile);
                    remove(coms);
                    rename(coms2, coms);
                }
            }
            else
            {
                fprintf(file, "%s\n", input);
                strcpy(arr[count], input);
                count++;
            }
            fclose(file);
        }
    }
}

void printhistory(char *input, char *dir, char *prev_dir)
{
    char *h;
    strcpy(coms, dir);
    strcat(coms, "/history.txt");
    h = strtok(input, " \n");
    h = strtok(NULL, " \n");
    if (h == NULL)
    {
        count = 0;
        if (access(coms, F_OK) == 0)
        {
            FILE *file = fopen(coms, "r");
            while (count < 15 && fscanf(file, "%4095[^\n]\n", arr[count]) != EOF)
            {
                count++;
            }
            for (int i = 0; i < count; i++)
            {
                printf("%s\n", arr[i]);
            }
            fclose(file);
        }
        else
        {
            printf("No commands executed yet\n");
        }
    }
    else if (strcmp(h, "purge") == 0)
    {
        count = 0;
        FILE *file = fopen(coms, "w");
        fclose(file);
    }
    else if (strcmp(h, "execute") == 0)
    {
        char *indexStr = strtok(NULL, " \n");
        if (indexStr != NULL)
        {
            int index = atoi(indexStr);
            if (index >= 1 && index <= count)
            {
                // system(arr[count - index]);
                if (access(coms, F_OK) == 0)
                {
                    FILE *file = fopen(coms, "r");
                    char command[4096];
                    int c = 0;
                    while (fgets(command, sizeof(command), file) != NULL)
                    {
                        c++;
                        if (c == count - index + 1)
                        {
                            // printf("%s\n",command);
                            break;
                        }
                    }
                    fclose(file);
                    command[strlen(command)-1]='\0';
                    // printf("%s\n",command);
                    append(command, dir);

                    char *x;
                    char *t = strtok_r(command, ";", &x);
                    // char* cd=strtok(input," \t\n");
                    char *shell[] = {"sh", "-c", NULL, NULL};
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
                        else if (strstr(t, "|") != NULL)
                        {
                            // printf("1\n");
                            pipe1(t, shell, dir, prev_dir);
                        }
                        else if (strstr(t, "<") != NULL)
                        {
                            
                            if (strstr(t, ">>") != NULL)
                            {
                                write1(t, 1, shell, dir, prev_dir);
                            }
                            else if (strstr(t, ">") != NULL)
                            {
                                write1(t, 0, shell, dir, prev_dir);
                            }
                            else
                            {
                                read1(t, shell, dir, prev_dir);
                            }
                        }

                        else if (strstr(t, ">>") != NULL)
                        {
                            
                            if (strstr(t, "<") != NULL)
                            {
                                read1(t, shell, dir, prev_dir);
                            }
                            else
                            {
                                write1(t, 1, shell, dir, prev_dir);
                            }
                        }

                        else if (strstr(t, ">") != NULL)
                        {
                            if (strstr(t, "<") != NULL)
                            {
                                read1(t, shell, dir, prev_dir);
                            }
                            else
                            {
                                write1(t, 0, shell, dir, prev_dir);
                            }
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
                                setpgid(0, 0);
                                execvp("/bin/bash", shell);
                            }
                            else
                            {
                                wait(NULL);
                            }
                        }
                        t = strtok_r(NULL, ";", &x);
                    }
                }
                else
                {
                    printf("No commands executed yet\n");
                }
            }
            // printf("%d\n",index);
        }
        else
        {
            printf("Error\n");
        }
    }
}
