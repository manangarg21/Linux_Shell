#include "headers.h"

void getinfo(char *input)
{
    char *t;
    int pid, gpid, count = 0, ch, i = 0, j = 0;
    unsigned int size;
    char *arr1 = (char *)malloc(4096);
    char *arr2 = (char *)malloc(4096);
    char *info = (char *)malloc(4096);
    char *execut = (char *)malloc(4096);
    char *exe = (char *)malloc(4096);

    t = strtok(input, " \t\n");
    // printf("%s\n",t);
    t = strtok(NULL, " \t\n");
    // if(t==NULL)
    // {
    //     printf("1\n");
    // }
    // else
    // {
    //     printf("2\n");
    // }
    if (t == NULL)
    {
        pid = getpid();
        // printf("%d\n",pid);
    }

    else
    {
        pid = atoi(t);
        // printf("%d\n",pid);
    }

    snprintf(info, 4096, "/proc/%d/stat", pid);

    FILE *file = fopen(info, "r");
    if (file == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return;
    }

    fseek(file, 0, SEEK_SET);
    ch = fgetc(file);
    while (ch != EOF)
    {
        if (ch == ' ')
        {
            count++;
        }
        if (count == 2)
        {
            break;
        }
        ch = fgetc(file);
    }

    char mode = fgetc(file);

    ch = fgetc(file);
    while (ch != EOF)
    {
        if (ch == ' ')
        {
            count++;
        }
        if (count == 4)
        {
            break;
        }
        ch = fgetc(file);
    }

    ch = fgetc(file);
    while (ch != EOF && ch != ' ')
    {
        arr1[i++] = (char)(ch);
        ch = fgetc(file);
    }
    arr1[i] = '\0';
    gpid = atoi(arr1);
    count++;
    free(arr1);

    ch = fgetc(file);
    while (ch != EOF)
    {
        if (ch == ' ')
        {
            count++;
        }
        if (count == 22)
        {
            break;
        }
        ch = fgetc(file);
    }

    ch = fgetc(file);
    while (ch != EOF && ch != ' ')
    {
        arr2[j++] = (char)(ch);
        ch = fgetc(file);
    }
    arr2[j] = '\0';
    size = atoi(arr2);
    free(arr2);

    fclose(file);

    snprintf(execut, 4096, "/proc/%d/exe", pid);
    int len = readlink(execut, exe, 4095);
    exe[len] = '\0';

    free(execut);

    printf("pid : %d\n", pid);
    printf("process status : %c", mode);
    if (mode == 'R' || mode == 'S')
    {
        printf("+\n");
    }
    else
    {
        printf("\n");
    }
    printf("Process Group : %d\n", gpid);
    printf("Virtual memory : %u\n", size);
    printf("executable path : %s\n", exe);

    free(exe);

    return;
}