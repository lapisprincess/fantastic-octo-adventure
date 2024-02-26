#pragma once
#define MAXBUF 256  // max number of characters allowed on command line
#define MAXARG 8    // max number of arguments

// TODO: Any global variables go below

// TODO: Function declarations go below
int runcmd(char** cmd);
int builtin(const char* command, char** cmdarr);
int resolvepth(char *out, char *cmd);
char** split(char *str, char *delim);
