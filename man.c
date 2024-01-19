#include "headers.h"

void manpage(char *t)
{
    struct addrinfo info;
    struct addrinfo *init;
    struct addrinfo *ptr;

    // char *t = (char *)malloc(4096);
    t = strtok(t, " \n");
    // printf("%s\n", t);

    char *url = (char *)malloc(4096);
    snprintf(url, 4096, "/?topic=%s&section=all", t);

    memset(&info, 0, sizeof(struct addrinfo));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo("man.he.net", "http", &info, &init);
    if (status != 0)
    {
        perror("Getaddrinfo failed");
    }

    int sockfd = -1;
    ptr = init;
    while (ptr != NULL)
    {
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sockfd == -1)
        {
            continue;
        }

        int conn = connect(sockfd, ptr->ai_addr, ptr->ai_addrlen);
        if (conn != -1)
        {
            break;
        }

        close(sockfd);
        ptr = ptr->ai_next;
    }

    if (ptr == NULL)
    {
        fprintf(stderr, "Failed to connect to the server\n");
        freeaddrinfo(init);
        return;
    }

    freeaddrinfo(init);

    char *request = (char *)malloc(4096);
    snprintf(request, 4096, "GET %s HTTP/1.1\r\nHost: man.he.net\r\n\r\n", url);
    int w = write(sockfd, request, strlen(request));
    if (w < 0)
    {
        perror("Failed to send GET request");
        close(sockfd);
        free(request);
        return;
    }

    char response[100000];
    ssize_t n;
    ssize_t rec_total = 0;
    // ssize_t n1=recv(sockfd, response, sizeof(response), 0);
    while ((n = recv(sockfd, response + rec_total, sizeof(response) - rec_total, 0)) > 0)
    {
        rec_total += n;
        if (rec_total >= 100000)
        {
            break;
        }
        // fwrite(response, 1, n, stdout);
    }
    response[rec_total] = '\0';
    // printf("%s\n",response);
    char *check = strstr(response, "No matches for");
    if (check != NULL)
    {
        printf("Error: No such command\n");
        return;
    }

    else
    {

        // printf("%s\n",response);
        char *start1 = strstr(response, "<PRE>");
        char *end1 = strstr(response, "</PRE>");

        if (start1 == NULL || end1 == NULL)
        {
            return;
        }

        size_t length1 = end1 - start1 - 5;
        char *data1 = (char *)malloc(length1 + 1);
        if (data1 == NULL)
        {
            perror("Memory allocation failed");
            return;
        }

        strncpy(data1, start1 + 5, length1);
        data1[length1] = '\0';
        // printf("%s\n",data1);

        // char temp[4096];
        // strcat(temp,t);
        // snprintf(temp, sizeof(temp), "NAME\n\t%s", t);
        // printf("%s\n", temp);
        char *start = strstr(data1, "NAME");
        // printf("%s\n",start);
        char *end_1 = strstr(data1, "AUTHOR");
        // printf("%s\n",end_1);
        char *end_2 = NULL;
        char *end_3 = NULL;
        char *end_4 = NULL;
        if (end_1 == NULL)
        {
            // printf("1\n");
            end_2 = strstr(data1, "RETURN VALUE");
            if (end_2 == NULL)
            {
                end_3 = strstr(data1, "OPTIONS");
            }
            if (end_3 == NULL)
            {
                end_4 = strstr(data1, "ATTRIBUTES");
            }
        }

        if (start == NULL || (end_1 == NULL && end_2 == NULL && end_3 == NULL && end_4 == NULL))
        {
            printf("Not in syntax\n");
            return;
        }
        size_t length = 0;
        if (end_1 != NULL)
        {
            length = end_1 - start;
        }
        else if (end_2 != NULL)
        {
            length = end_2 - start;
        }
        else if (end_3 != NULL)
        {
            length = end_3 - start;
        }
        else
        {
            length = end_4 - start;
        }
        printf("%ld\n", length);
        char *data = (char *)malloc(length + 1);
        if (data == NULL)
        {
            perror("Memory allocation failed");
            return;
        }

        strncpy(data, start, length);
        data[length] = '\0';

        printf("%s\n", data);

        free(data1);
        free(data);

        free(request);
        free(url);
    }
    close(sockfd);
}