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
// return 1 if command not found, 2 if execv didn't work out
int runcmd(char** cmdarr) {
    char *command = cmdarr[0];

    // built-in commands
    if (builtin(command, cmdarr) == 0) {
        free(cmdarr);
        return 0;
    }

    // process argv
    char *argv[MAXARG];
    int shouldwait = 0;
    for (int i = 0; i < MAXARG; i++) {
        if (strcmp(cmdarr[i], "&") == 0) shouldwait = 1;
        if (strcmp(cmdarr[i], "") == 0) argv[i] = NULL;
        else argv[i] = cmdarr[i];
    }
    argv[MAXARG-1] = NULL;

    // find path to process
    char *fullpth = (char*) malloc(MAXBUF);
    if (resolvepth(fullpth, command) == 1) {
        printf("ERROR: process %s not found!\n", command);
        free(fullpth);
        free(cmdarr);
        return 1;
    }

    // fork and exec
    int pid = fork();
    if ((pid == 0) && (execv(fullpath, argv) == -1)) { // child and failing
        printf("ERROR: unable to run process %s!\n", command);
        return 2;
    } else if (!shouldwait) { // parent and waiting
        wait(NULL);
    }

    free(fullpth);
    free(cmdarr);
    return 0;
}


// check if command is built-in. 
// return 0 if it is, 1 otherwise.
int builtin(const char* command, char** cmdarr) {
    if (strcmp(command, "pwd") == 0) {
        char *buf = (char*) malloc(MAXBUF);
        getcwd(buf, MAXBUF);
        printf("%s\n", buf);
        free(buf);
    } else if (strcmp(command, "cd") == 0) {
        char *ndir;
        if (strcmp(cmdarr[1], "") == 0) ndir = getenv("HOME");
        else ndir = cmdarr[1];
        if (chdir(ndir)) printf("Directory not found!\n");
    } else if (strcmp(command, "exit") == 0) {
        printf("Goodbye!\n");
    } else if (strcmp(command, "") == 0) {
        printf("Please enter a command!\n");
    } else return 1;

    return 0;
}


// resolve path to specified command, storing resolved path in out.
// returns 1 if path coulnd't be resolved.
int resolvepth(char *out, char *cmd) {

    // full path given! <3
    if ((cmd[0] == '/') && (access(cmd, F_OK | X_OK) == 0)) {
        strcpy(out, cmd);
        return 0;
    }

    // look in cwd
    char *path = (char*) malloc(MAXBUF * sizeof(char));
    getcwd(path, MAXBUF);
    strcat(path, "/");
    strcat(path, cmd);
    if (access(path, F_OK | X_OK) == 0) {
        strcpy(out, path);
        free(path);
        return 0;
    }

    // go digging
    char *buf = (char*) malloc(MAXBUF * sizeof(char));
    strcpy(path, getenv("PATH"));
    path = strtok(path, ":");
    while (path != NULL) {
        strcpy(buf, path);
        strcat(buf, "/");
        strcat(buf, cmd);
        if (access(buf, F_OK | X_OK) == 0) {
            strcpy(out, buf);
            free(buf);
            return 0;
        }
        path = strtok(NULL, ":");
    } 

    // no luck :(
    free(path);
    free(buf);
    return 1;
}



// given a string, return array of strings split at delimeter
char** split(char *str, char *delim) {
    int word_i = 0, out_i = 0, outword_i = 0;
    char c;
    char **out = (char**) malloc(MAXBUF * sizeof(char*));

    for (int i = 0; i < MAXARG; i++)
        out[i] = (char*) malloc(MAXBUF * sizeof(char*));

    while (1) {
        c = str[word_i++];
        if (c == '\n') break;
        else if (c == ' ') {
            out_i++;
            if (out_i >= MAXARG-1) break;
            outword_i = 0;
            continue;
        }
        out[out_i][outword_i++] = c;
    }

    return out;
}
