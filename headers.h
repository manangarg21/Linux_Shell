#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <termio.h>
#include <termios.h>

#include "prompt.h"
#include "activity.h"
#include "bg.h"
#include "fg.h"
#include "man.h"
#include "neonate.h"
#include "pastevents.h"
#include "peek.h"
#include "ping.h"
#include "pipe.h"
#include "proc.h"
#include "redirect.h"
#include "seek.h"
#include "warp.h"

typedef struct ProcessInfo
{
    pid_t pidval;
    char *cmdinfo;
    char state;
} pinfo;

#endif