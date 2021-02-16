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

static char *dest = "/tmp/jutmp.json";

int main() {

        int val = setjmp(jmp_buffer);
        if (val) {
                Message("FATAL: %s:%s, %d Cannot continue, will exit!\n", __FILE__, __func__, __LINE__)
                exit(100);
        }

        // int parse_res = argp_parse(0, argc, argv, 0, 0, 0);

        FILE * json_file = fopen(dest, "w");
        if (! json_file) {
                Message("ERROR: %s,%d: Opening file %s: %s\n", __func__, __LINE__, WTMP_FILE, strerror(errno))
                longjmp(jmp_buffer, 100);
        }
        Message("INFO: Opened '%s' for reading (saving results to %s)\n", WTMP_FILE, dest)
        print_utmp(json_file);
        fclose(json_file);
        return (0);
}