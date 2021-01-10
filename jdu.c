/*
 * Disk utilization dump:
 * [josevnz@mac-pro-1-1 jdumpertools]$ ./jdu  /
 * [
 *   {
 *      "free_space": 52576092160.0,
 *      "name": "/",
 *      "total_space": 31590506496.0
 *   }
 * ]
 */
#include <sys/statvfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>

#include "jdumpertools.h"

jmp_buf buffer;

int disk_details(int num_paths, char **paths, FILE * json_file) {

    DEBUG_PRINT("num_paths=%d\n", num_paths)
    fprintf(json_file, "[");
    for(int i =1; i < num_paths; i++) {
        char * path = paths[i];
        DEBUG_PRINT("path=%s\n", paths[i])
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
        fprintf(json_file, "{\"partition\": \"%s\", \"free_space\": %f, \"total_space\": %f}", path, free_space, total_space);
        if (i< num_paths -1) {
            fprintf(json_file, ",");
        }
    } // end loop
    fprintf(json_file, "]\n");
    return 0;
}

int main(int argc, char *argv[]) {
    
    int val = setjmp(buffer);
    if (val) {
        Message("ERROR: %s:%s:%d: Cannot recover, program will exit with an error now.\n", __FILE__, __func__, __LINE__)
        exit(val);
    }

    // int parse_res = argp_parse(0, argc, argv, 0, 0, 0);

    if (argc == 1) {
        Message("ERROR: %s:%d: Need to pass one or more directories to check!\n", __func__, __LINE__)
        longjmp(buffer, 100);
    }
    
    
    disk_details(argc, argv, stdout);
    
    return 0;
}