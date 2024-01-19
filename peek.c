#include "headers.h"

void permissions(mode_t perm)
{
    printf((S_ISDIR(perm)) ? "d" : "-");
    printf((perm & S_IRUSR) ? "r" : "-");
    printf((perm & S_IWUSR) ? "w" : "-");
    printf((perm & S_IXUSR) ? "x" : "-");
    printf((perm & S_IRGRP) ? "r" : "-");
    printf((perm & S_IWGRP) ? "w" : "-");
    printf((perm & S_IXGRP) ? "x" : "-");
    printf((perm & S_IROTH) ? "r" : "-");
    printf((perm & S_IWOTH) ? "w" : "-");
    printf((perm & S_IXOTH) ? "x" : "-");
}

void ls1(const struct stat *fi, char *file)
{
    if (S_ISDIR(fi->st_mode))
    {
        printf("\x1b[34m");
    }
    else if (fi->st_mode & S_IXUSR || fi->st_mode & S_IXGRP || fi->st_mode & S_IXOTH)
    {
        printf("\x1b[32m");
    }

    permissions(fi->st_mode);
    printf(" %4lu ", fi->st_nlink);
    struct passwd *user = getpwuid(fi->st_uid);
    struct group *group = getgrgid(fi->st_gid);
    printf(" %s %s ", user->pw_name, group->gr_name);
    printf(" %8lld ", (long long)fi->st_size);
    char date[20];
    strftime(date, sizeof(date), "%b %d %H:%M", localtime(&fi->st_mtime));
    printf("%s ", date);
    printf("%s%s\n", file, "\x1b[0m");
}

int a = 0;
int l = 0;
int cnt = 0;

void flags(char *input)
{
    if (strstr(input, "-l") != NULL)
    {
        l = 1;
        // a = 0;
        if (strstr(input, "-la") != NULL)
        {
            a = 1;
        }
    }
    if (strstr(input, "-a") != NULL)
    {
        a = 1;
        // l = 0;
        if (strstr(input, "-al") != NULL)
        {
            l = 1;
        }
    }
}

void peek_terminal(char *input, char *dir)
{
    a=0,l=0;
    flags(input);
    // printf("%d %d\n", a, l);
    char *ls;
    long long total = 0;
    struct dirent **curr_dir;
    // printf("%s\n",input);
    // printf("1\n");
    char *exec = (char *)malloc(4096);
    char *temp = (char *)malloc(4096);
    char *temp2 = (char *)malloc(4096);
    ls = strtok(input, " \n");
    char *prev;
    while (ls != NULL)
    {
        prev = ls;
        ls = strtok(NULL, " \n");
    }
    // printf("%s\n",prev);
    // ls = strtok(NULL, " \n");
    // printf("%s\n",ls);
    // printf("%c\n",ls[1]);
    // printf("%d\n",strlen(ls));
    // printf("2\n");
    if (strstr(prev, "-") != NULL || strcmp(prev, "peek") == 0)
    {
        getcwd(exec, 4096);
    }
    else if (strcmp(prev, "~") == 0)
    {
        strcpy(exec, dir);
    }
    else
    {
        strcpy(exec, prev);
    }
//    printf("%s\n", exec);
    int prt = scandir(exec, &curr_dir, NULL, alphasort);

    if (prt == -1)
    {
        perror("scandir");
        return;
    }

    if (l == 1 && a==1)
    {
        for (int i = 0; i < prt; i++)
        {
            char *path = (char *)malloc(4096);
            snprintf(path, 4096, "%s/%s", exec, curr_dir[i]->d_name);
            struct stat fi;
            if (stat(path, &fi) == 0)
            {
                total += fi.st_blocks;
            }
        }
        printf("total %lld\n", total);
    }
    else if(l==1 && a==0)
    {
        for (int i = 0; i < prt; i++)
        {
            char *path = (char *)malloc(4096);
            if(curr_dir[i]->d_name[0]!='.')
            {
                snprintf(path, 4096, "%s/%s", exec, curr_dir[i]->d_name);
            }
            struct stat fi;
            if (stat(path, &fi) == 0)
            {
                total += fi.st_blocks;
            }
        }
        printf("total %lld\n", total);
    }

    for (int i = 0; i < prt; i++)
    {
        if (l == 0)
        {
            if (curr_dir[i]->d_name[0] == '.' && a == 1)
            {
                printf("%s\n", curr_dir[i]->d_name);
            }

            else if (curr_dir[i]->d_name[0] != '.')
            {
                printf("%s\n", curr_dir[i]->d_name);
            }
        }

        else
        {
            struct stat fi;
            char *path = (char *)malloc(4096);
            snprintf(path, 4096, "%s/%s", exec, curr_dir[i]->d_name);
            if (lstat(path, &fi) == -1)
            {
                perror("lstat");
                continue;
            }

            if (curr_dir[i]->d_name[0] == '.' && a == 1)
            {
                if (lstat(path, &fi) == -1)
                {
                    perror("lstat");
                    continue;
                }
                ls1(&fi, curr_dir[i]->d_name);
            }

            else if (curr_dir[i]->d_name[0] != '.')
            {
                if (lstat(path, &fi) == -1)
                {
                    perror("lstat");
                    continue;
                }
                ls1(&fi, curr_dir[i]->d_name);
            }
        }
    }
}