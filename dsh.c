/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>


// run process based on given array of command and parameters
void runcmd(char** cmdarr) {
    if (strcmp(cmdarr[0], "pwd") == 0) {
        char *buf = (char*) malloc(MAXBUF);
        getcwd(buf, MAXBUF);
        printf("%s\n", buf);
    } else if (strcmp(cmdarr[0], "cd") == 0) {
        if (strcmp(cmdarr[1], "") == 0) chdir(getenv("HOME"));
        else {
            chdir(cmdarr[1]);
        }
    } else if (strcmp(cmdarr[0], "") == 0) {
        printf("Please enter a command!\n");
    } else if (strcmp(cmdarr[0], "exit") == 0) {
        printf("Goodbye!\n");
    } else printf("ERROR: %s not found!\n", cmdarr[0]);

    free(cmdarr);
}


// given a string, return array of strings split at delimeter
char** split(char *str, char *delim) {
    char **out = (char**) malloc(MAXBUF * sizeof(char*));
    for (int i = 0; i < MAXARG; i++)
        out[i] = (char*) malloc(MAXBUF * sizeof(char*));

    int word_i = 0, out_i = 0, outword_i = 0;
    char c = str[0];
    while (c != NULL) {
        c = str[word_i++];
        if (c == '\n') break;
        if (c == ' ') {
            out_i++;
            outword_i = 0;
            continue;
        }
        out[out_i][outword_i++] = c;
    }

    return out;
}
