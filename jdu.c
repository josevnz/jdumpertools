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
#include "jdumpertools.h"

int main(int argc, char *argv[]) {
    
    int val = setjmp(jmp_buffer);
    if (val) {
        Message("ERROR: %s:%s:%d: Cannot recover, program will exit with an error now.\n", __FILE__, __func__, __LINE__)
        exit(val);
    }

    // int parse_res = argp_parse(0, argc, argv, 0, 0, 0);

    if (argc == 1) {
        Message("ERROR: %s:%d: Need to pass one or more directories to check!\n", __func__, __LINE__)
        longjmp(jmp_buffer, 100);
    }
    
    disk_details(argc, argv, stdout);
    
    return 0;
}