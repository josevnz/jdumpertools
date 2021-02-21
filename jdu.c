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

static const int MAX_ALARM_WAIT = 60;

int main(int argc, char *argv[]) {
    
    int val = setjmp(jmp_buffer);
    if (val) {
        Message("ERROR: %s:%s:%d: Cannot recover, program will exit with an error now.\n", __FILE__, __func__, __LINE__)
        exit(val);
    }

    void (*sigHandlerReturn) (int);

    sigHandlerReturn = signal(SIGALRM, alarmHandler);
    if (sigHandlerReturn == SIG_ERR) {
        perror("Signal error!");
        longjmp(jmp_buffer,100);
    }
    signal(SIGHUP, ignoreNohupHandler);
        if (sigHandlerReturn == SIG_ERR) {
        perror("Signal error!");
        longjmp(jmp_buffer,100);
    }

    char *file_name = NULL;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "f:")) != -1)
        switch (c) {
            case 'f':
            file_name = optarg;
            break;
        case '?':
            if (optopt == 'f') {
                Message("Option -%c requires an argument.\n", optopt)
            } else if (isprint(optopt)) {
                Message("Unknown option `-%c'.\n", optopt)
            } else {
                Message("Unknown option character `\\x%x'.\n", optopt)
                return 100;
            }
            break;
        default:
            abort();
    }

    if ((argc-optind) < 1) {
        Message("ERROR: %s:%d: Need to pass ONE or more directories to check...!\n", __func__, __LINE__)
        longjmp(jmp_buffer, 100);
    }
    
    alarm(MAX_ALARM_WAIT);

    FILE * destination_file = stdout;
    if (file_name != NULL ) {
        destination_file = fopen(file_name, "w");
        if (! destination_file) {
            Message("ERROR: %s,%d: Opening file %s: %s\n", __func__, __LINE__, file_name, strerror(errno))
            longjmp(jmp_buffer, 100);
        }
    }

    disk_details(argc, argv, destination_file, optind);
    alarm(0);
    return 0;
}