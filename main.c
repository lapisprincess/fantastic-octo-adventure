/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"

#define MAX_PROC 250

int main(int argc, char *argv[]) {

	// DO NOT REMOVE THE BLOCK BELOW (FORK BOMB PREVENTION) //
	struct rlimit limit;
	limit.rlim_cur = MAX_PROC;
	limit.rlim_max = MAX_PROC;
	setrlimit(RLIMIT_NPROC, &limit);
	// DO NOT REMOVE THE BLOCK ABOVE THIS LINE //

        char *cmdline = (char*) malloc(MAXBUF * sizeof(char));
        do {
            printf("dsh> ");
            fgets(cmdline, MAXBUF, stdin);
            if (runcmd(split(cmdline, " ")) == 2) break;
        } while (strcmp(cmdline, "exit\n") != 0);

        free(cmdline);
	return 0;
}
