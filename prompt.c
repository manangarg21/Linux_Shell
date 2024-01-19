#include "headers.h"
extern int y;
extern char str_global[4096];

void prompt(char *input)
{
    // Do not hardcode the prmopt
    // printf("<Everything is a file> ");
    char *str = (char *)malloc(4096);
    char *host = (char *)malloc(4096);
    char *user = getlogin();
    char *fg = (char *)malloc(4096);
    getcwd(str, 4096);
    gethostname(host, 4096);
    // char home[]="/home/manan21/mini-project-1-manangarg21";
    // printf("df%s\n%s\n",home,str);

    if (y > 0)
    {
        fg = strtok(str_global, " \n");
        // printf("%s\n",fg);
        char *t = (char *)malloc(4096);
        sprintf(t, "%d", y);
        strcat(fg, " : ");
        strcat(fg, t);
        strcat(fg,"s");

        if (strncmp(str, input, strlen(input)) == 0)
        {
            char *sub = str + strlen(input);
            printf("<%s@%s:~%s %s> ", user, host, sub, fg);
        }
        else
        {
            printf("<%s@%s:%s %s> ", user, host, str, fg);
        }
        // printf("%d\n",eta);
        // strcat(input," : ");
        // strcat(input,t);
        // strcat(dir,input);
    }
    else
    {
        if (strncmp(str, input, strlen(input)) == 0)
        {
            char *sub = str + strlen(input);
            printf("<%s@%s:~%s> ", user, host, sub);
        }
        else
        {
            printf("<%s@%s:%s> ", user, host, str);
        }
    }
}
