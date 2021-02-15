#include <sys/statvfs.h>
#include "jdumpertools.h"
#include <setjmp.h>

int disk_details(int num_paths, char **paths, FILE * json_file) {
    struct statvfs results;
    DEBUG_PRINT("num_paths=%d\n", num_paths)
    fprintf(json_file, "[");
    for(int i =1; i < num_paths; i++) {
        char * path = paths[i];
        DEBUG_PRINT("path=%s\n", paths[i])
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