#include "headers.h"

/////////////////BOILERPLATE///////////////////////

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/////////////////BOILERPLATE///////////////////////

int recentpid()
{
    int recent_id;
    FILE *file = fopen("/proc/loadavg", "r");

    if (file==NULL)
    {
        perror("File not found");
    }
    else
    {
        char *line = (char *)malloc(4096);
        if(fread(line, 1, 4095, file)>0);
        char *t = strtok(line, " ");
        t = strtok(NULL, " ");
        t = strtok(NULL, " ");
        t = strtok(NULL, " ");
        t = strtok(NULL, " \n");
        recent_id = atoi(t);
    }

    fclose(file);
    return recent_id;
}

void neonate(char *input)
{
    char *t = strtok(input, " \n\t");
    t = strtok(NULL, " \n\t");
    t = strtok(NULL, " \n\t");
    int in = atoi(t);
    char end;
    int fk = fork();

    if (fk == 0)
    {

        // printf("%d\n",in);
        while (1)
        {
            // fflush(stdout);
            printf("%d\n", recentpid());
            sleep(in);
        }
    }
    else
    {
        enableRawMode();
        while (read(STDIN_FILENO, &end, 1) == 1)
        {
            if (end == 'x')
            {
                kill(fk, SIGTERM);
                break;
            }
        }
    }
    disableRawMode();
    return;
}
