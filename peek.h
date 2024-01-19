#ifndef __PEEK_H
#define __PEEK_H

void permissions(mode_t perm);
void ls1(const struct stat *fi, char *file);
void flags(char *input);
void peek_terminal(char *input, char *dir);

#endif