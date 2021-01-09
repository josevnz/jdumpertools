/*
 * Simple DU program
 */
#include <sys/statvfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>

#include "jdumpertools.h"

jmp_buf buffer;

int disk_details(char *path) {
    struct statvfs results;
    if (statvfs(path, &results) == -1) {
        int error_n = errno;
        if (error_n == ENOENT) {
            Message("ERROR: %s:%d, Code: %d\n", __func__, __LINE__, error_n)
            perror(path);
        } else {
            perror(path);
        }
        longjmp(buffer, 100);
    }
    double free_space = (double) results.f_bavail * results.f_frsize;
    double total_space = (double) results.f_blocks * results.f_frsize;
    fprintf(stdout, "INFO: '%s' free space %.3f GB/%.3f GB\n", path, free_space/1024.0/1024.0/1024.0, total_space/1024.0/1024.0/1024.0);
    return 0;
}

int main(int argc, char *argv[]) {
    
    int val = setjmp(buffer);
    if (val) {
        Message("ERROR: %s:%s:%d: Cannot recover, program will exit with an error now.\n", __FILE__, __func__, __LINE__);
        exit(val);
    }

    if (argc == 1) {
        Message("ERROR: %s:%d: Need to pass one or more directories to check!\n", __func__, __LINE__);
        longjmp(buffer, 100);
    }

    for (int i = 1; i < argc; i++) {
        disk_details(argv[i]);
    }
    return 0;
}