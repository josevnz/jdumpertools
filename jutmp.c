/*
 * Read utmp log and create JSON document from it.
 *   {
 *   "timestamp": "Fri Sep  4 04:31:58 2020",
 *   "user": "josevnz",
 *   "pid": 1678,
 *   "tty": ":0",
 *   "session_id": 0,
 *   "type_of_record": "NormalProcess",
 *   "kernel": ":0",
 *   "exit": {
 *     "termination": 0,
 *     "exit code": 0
 *   },
 *   "ip_addr": "0.0.0.0"
 *   }
 * ]
 * Author: Jose Vicente Nunez
 */
#include "jdumpertools.h"

static const int MAX_ALARM_WAIT = 60;

int main(int argc, char *argv[]) {

        int val = setjmp(jmp_buffer);
        if (val) {
                Message("FATAL: %s:%s, %d Cannot continue, will exit!\n", __FILE__, __func__, __LINE__)
                exit(100);
        }

        signal(SIGALRM, alarmHandler);

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

        alarm(MAX_ALARM_WAIT);
        FILE * destination_file = stdout;
        if (file_name != NULL ) {
                destination_file = fopen(file_name, "w");
                if (! destination_file) {
                        Message("ERROR: %s,%d: Opening file %s: %s\n", __func__, __LINE__, file_name, strerror(errno))
                        longjmp(jmp_buffer, 100);
                }
                Message("INFO: Opening '%s' for reading (saving results to %s)\n", WTMP_FILE, file_name)
        } else {
                Message("INFO: Opening '%s' for reading (saving results to STDOUT)\n", WTMP_FILE)
        }
        print_utmp(destination_file);
        fclose(destination_file);
        alarm(0);
        return (0);
}