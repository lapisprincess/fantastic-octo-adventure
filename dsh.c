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
    } else printf("ERROR: %s not found!\n", cmdarr[0]);
    free(cmdarr);
}

char** split(char *str, char *delim) {
    // initialize array of strings
    char **out = (char**) malloc(MAXBUF * sizeof(char*));
    for (int i = 0; i < MAXARG; i++)
        out[i] = (char*) malloc(MAXBUF * sizeof(char*));

    char *buf = (char*) malloc(MAXBUF * sizeof(char*));
    int i = 0;
    int j = 0;
    char c = str[0];
    while (c != NULL) {
        c = str[i];
        if (c == '\n') break;
        if (c == ' ') {
            strcpy(out[j], buf);
            memset(buf, '\0', sizeof(buf));
            j++;
            continue;
        }
        strncat(buf, &c, 1);
        i++;
    }
    strcpy(out[j], buf);
    return out;
}

