#pragma once
#define MAXBUF 256  // max number of characters allowed on command line
#define MAXARG 8    // max number of arguments

// TODO: Any global variables go below

// TODO: Function declarations go below
void runcmd(char** cmd);
char** split(char *str, char *delim);
