#ifndef __ACTIVITY_H
#define __ACTIVITY_H

char *commandname(pid_t forkid);
char getstate(pid_t forkid);
int compare_processes(const void *a, const void *b);
void populate_structs();
void activity();
#endif