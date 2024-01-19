#include "headers.h"

pinfo processes[4096];
int num_processes = 0;

char *commandname(pid_t forkid)
{
    char name[4096];
    snprintf(name, sizeof(name), "/proc/%d/cmdline", forkid);
    FILE *file = fopen(name, "r");
    if (file != NULL)
    {
        static char cmd[4096];
        fgets(cmd, sizeof(cmd), file);
        fclose(file);
        return cmd;
    }
    else
    {
        perror("Couldnt open file");
        return NULL;
    }
}

char getstate(pid_t forkid)
{
    char name[4096];
    char ch;
    int count = 0;
    snprintf(name, 4096, "/proc/%d/stat", forkid);
    FILE *file = fopen(name, "r");
    char line[4096];
    if (file == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return '\0';
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
    return mode;
}

// Function to compare two ProcessInfo structs for sorting
int compare_processes(const void *a, const void *b)
{
    return ((pinfo *)a)->pidval - ((pinfo *)b)->pidval;
}

void populate_structs()
{
    num_processes = 0;
    struct dirent **file;
    int dir = scandir("/proc", &file, NULL, NULL);
    // printf("1\n");
    if (dir < 0)
    {
        perror("Wrong or Empty directory!");
        return;
    }

    // Create an array to store information about processes
    // pinfo *processes = (pinfo *)malloc(sizeof(pinfo) * 4096);
    // printf("1\n");

    for (int i = 0; i < dir; i++)
    {
        pid_t val = atoi(file[i]->d_name);
        // printf("1\n");
        // if(val==0)
        // {
        //     setpgid(0,0);
        // }
        if (val > 0)
        {
            // printf("1\n");
            char path[4096];
            snprintf(path, sizeof(path), "/proc/%d/stat", val);
            // printf("1\n");
            FILE *fi = fopen(path, "r");
            // printf("1\n");
            if (fi != NULL)
            {
                int x;
                fscanf(fi, "%*d %*s %*c %d", &x);
                fclose(fi);
                // printf("1\n");
                if (x == getpid())
                {
                    // printf("1\n");
                    char *command_name = commandname(val);
                    char stat = getstate(val);
                    // printf("1\n");
                    if (stat != 'K' || stat != 'Z')
                    {
                        // Store information about the process
                        processes[num_processes].pidval = val;
                        // strcpy(processes[num_processes].cmdinfo, command_name);
                        processes[num_processes].cmdinfo = strdup(command_name);
                        processes[num_processes].state = stat;
                        num_processes++;
                    }
                }
            }
        }
    }
}

void activity()
{
    // for (int i = 0; i < 4096; i++) {
    //     processes[i].pidval = -1;
    //     processes[i].cmdinfo = NULL;
    //     processes[i].state = '\0';
    // }
    populate_structs();
    // Sort the array of processes based on PIDs
    qsort(processes, num_processes, sizeof(pinfo), compare_processes);
    // printf("1\n");
    // Print the list of processes
    for (int i = 0; i < num_processes; i++)
    {
        // printf("%d : %s - ", processes[i].pidval, processes[i].cmdinfo);

        if (processes[i].state == 'T')
        {
            // printf("%c\n",processes[i].state);
            printf("%d : %s - Stopped\n", processes[i].pidval, processes[i].cmdinfo);
        }
        else if (processes[i].state == 'K' || processes[i].state == 'Z')
        {
            // printf("%c\n",processes[i].state);
            continue;
        }
        else
        {
            // printf("%c\n",processes[i].state);
            printf("%d : %s - Running\n", processes[i].pidval, processes[i].cmdinfo);
        }
    }
}
