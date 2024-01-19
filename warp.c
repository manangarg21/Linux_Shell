#include "headers.h"

void warp_dir(char *input,char* dir,char* prev_dir)
{
    char* cd;
    cd=strtok(input, " \t\n");
    cd=strtok(NULL," \t\n");

    if(cd==NULL)
    {
        getcwd(prev_dir,4096);
        chdir(dir);
        char *curr_dir = (char *)malloc(4096);
        getcwd(curr_dir, 4096);
        printf("%s\n", curr_dir);

    }

    while (cd != NULL)
    {
        // printf("%s\n",prev_dir);
        if (strcmp(cd, "..") == 0)
        {
            // char* parent=strdup(dir);
            // char* last=strrchr(parent,'/');
            // if(last!=NULL)
            // {
            //     *last='\0';
            //     chdir(parent);
            // }
            getcwd(prev_dir,4096);
            chdir("..");
            char *curr_dir = (char *)malloc(4096);
            getcwd(curr_dir, 4096);
            printf("%s\n", curr_dir);
        }
        else if (strcmp(cd, ".") == 0)
        {
            char *curr_dir = (char *)malloc(4096);
            getcwd(curr_dir, 4096);
            getcwd(prev_dir,4096);
            printf("%s\n", curr_dir);
        }
        else if (strcmp(cd, "-") == 0)
        {
            // printf("123\n");
            char* copy=(char *)malloc(4096);
            getcwd(copy, 4096);
            chdir(prev_dir);
            strcpy(prev_dir,copy);
            char *curr_dir = (char *)malloc(4096);
            getcwd(curr_dir,4096);
            printf("%s\n", curr_dir);
            free(copy);
        }
        else if (strcmp(cd, "~") == 0)
        {
            getcwd(prev_dir,4096);
            chdir(dir);
            char *curr_dir = (char *)malloc(4096);
            getcwd(curr_dir, 4096);
            printf("%s\n", curr_dir);
        }
        else if(strstr(cd,"~")!=NULL)
        {
            getcwd(prev_dir,4096);
            char* new=cd+2;
            chdir(new);
            char *curr_dir = (char *)malloc(4096);
            getcwd(curr_dir, 4096);
            printf("%s\n", curr_dir);
            free(new);
        }
        else
        {
            getcwd(prev_dir,4096);
            int a = chdir(cd);
            char *curr_dir = (char *)malloc(4096);
            getcwd(curr_dir, 4096);
            printf("%s\n", curr_dir);
            // printf("78%d\n",a);
        }
        cd=strtok(NULL," \t\n");
    }
}