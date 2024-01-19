#include "headers.h"

int ec;
char es[4096];

void seek(char *search, char *target, int d, int e, int f, char *rel)
{
    if (d == 1 && f == 1)
    {
        printf("Invalid flags!\n");
        return;
    }

    struct dirent **list;
    int num = scandir(target, &list, NULL, alphasort);
    if (num == -1)
    {
        perror("scandir");
        return;
    }

    for (int i = 0; i < num; ++i)
    {
        struct dirent *sub = list[i];

        if (strcmp(sub->d_name, ".") == 0 || strcmp(sub->d_name, "..") == 0)
        {
            free(sub);
            continue;
        }
        char newPath[4096];
        snprintf(newPath, sizeof(newPath), "%s/%s", rel, sub->d_name);
        char cppy[4096];
        strcpy(cppy,sub->d_name);
        char* fk=strtok(cppy,".");
        if (strcmp(fk, search) == 0)
        {
            if (sub->d_type == DT_DIR && f == 0)
            {
                printf("\033[34m.%s\033[0m\n", newPath); // Blue for directories
                if (e && ec == 0)
                {
                    strcpy(es, sub->d_name);
                    ec++;
                }
                else
                {
                    ec++;
                }
            }
            else if (sub->d_type == DT_REG && d == 0)
            {
                printf("\033[32m.%s\033[0m\n", newPath); // Green for files
                if (e && ec == 0)
                {
                    strcpy(es, sub->d_name);
                    ec++;
                }
                else
                {
                    ec++;
                }
            }
        }

        if (sub->d_type == DT_DIR)
        {
            char new[4096];
            snprintf(new, sizeof(new), "%s/%s", target, sub->d_name);
            seek(search, new, d, e, f, newPath);
        }

        free(sub);
    }

    free(list);
}

void seek_execute(char *input,char* dir)
{
    // printf("%s\n",input);
    int d = 0, e = 0, f = 0;
    ec = 0;
    char *target = (char *)malloc(4096);
    char *search = (char *)malloc(4096);
    int count_flags = 0;
    // printf("%s\n",input);
    char *tok=(char *)malloc(4096);
    // printf("%s\n",input);
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == '-')
        {
            count_flags++;
        }
    }
    count_flags++;
    // printf("%d\n",count_flags);
    tok = strtok(input, " \n");
    tok = strtok(NULL, " \n");
    while (count_flags > 0)
    {
        if (strstr(tok, "-d") != NULL)
        {
            d = 1;
            count_flags--;
            tok = strtok(NULL, " \n");
        }
        else if (strstr(tok, "-e") != NULL)
        {
            e = 1;
            count_flags--;
            tok = strtok(NULL, " \n");
        }
        else if (strstr(tok, "-f") != NULL)
        {
            f = 1;
            count_flags--;
            tok = strtok(NULL, " \n");
        }
        else
        {
            break;
        }
    }

    strcpy(search, tok);
    // printf("%s target\n", target);
    tok = strtok(NULL, "\n");
    if (tok != NULL)
    {
        if(strstr(tok,"~")==NULL)
        {
            strcpy(target, tok);
            count_flags--;
        }
        else
        {
            if(strcmp(tok,"~")==0)
            {
                strcpy(target,dir);
            }
            else
            {   
                target=tok+2;
            }
        }
    }
    else
    {
        // printf("here\n");
        target = ".";
        // target=getcwd(target,4096);
    }

    // printf("%s\n", search);
    // printf("%s\n", target);
    // printf("%d %d\n", d, f);
    seek(search, target, d, e, f, "");
    // printf("%d %d\n",e,ec);
    if (ec == 0)
    {
        printf("No match found!\n");
    }

    if (e == 1 && ec == 1)
    {
        char res[8192];
        snprintf(res, sizeof(res), "%s/%s", target, es);
        strcpy(es, res);
        // printf("%s\n",es);
        struct stat file_info;
        if (stat(es, &file_info) == 0)
        {
            if (S_ISDIR(file_info.st_mode)) // representing lone driectory found
            {
                if (access(es, X_OK) == 0)
                {
                    // printf("Change dir %s\n", es);
                    chdir(es);
                }
                else
                {
                    printf("Missing permissions for the task\n");
                    return;
                }
            }

            else
            {
                if (access(es, R_OK) == 0)
                {
                    // output_fcontents(es);
                    FILE *file = fopen(es, "r"); 
                    if (file == NULL)
                    {
                        perror("Error opening file");
                        return;
                    }

                    char buffer[4096];
                    size_t bytesRead;

                    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
                    {
                        fwrite(buffer, 1, bytesRead, stdout); 
                    }
                    printf("\n");
                    fclose(file);
                }
                else
                {
                    printf("Missing permissions for the task\n");
                    return;
                }
            }
        }
    }
}