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
 */
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <argp.h>
#include <stdbool.h>

#include "jdumpertools.h"

jmp_buf jmp_buffer;

static char *filename = "/var/log/wtmp";
static char *dest = "/tmp/jutmp.json";
static const int LOG_SIZE = 5000;

int print_utmp(FILE * json_file) {

        int file;
        struct utmp log[LOG_SIZE];
        int i = 0;
        DEBUG_PRINT("filename=%s\n", filename)
        file = open(filename, O_RDONLY);
        if (file < 0) {
                Message("ERROR: %s:%d, Failed to open\n", __func__, __LINE__)
                longjmp(jmp_buffer, 100);
        }
        if (file) {
                int bytes_read = read(file, &log, LOG_SIZE * sizeof(struct utmp));
                DEBUG_PRINT("UTMP bytes read: %d\n", bytes_read);
                if (bytes_read < 0) {
                        Message("ERROR: %s:%d, Failed to open\n", __func__, __LINE__)
                        longjmp(jmp_buffer, 100);
                }
                char terminal[UT_LINESIZE + 1];
                char host[UT_HOSTSIZE + 1];
                char buffer[BUFFER_SIZE];

                fprintf(json_file, "[");
                for (i = 0; i < LOG_SIZE; i++) {
                        utmpprint(i, &log[i], terminal, host, buffer, json_file);
                }
                fprintf(json_file, "]\n");
                close(file);
        } else {
                Message("ERROR: %s, %d: No UTMP file!\n", __func__, __LINE__)
                longjmp(jmp_buffer, 100);
        }
        return 0;
}

int main() {

        int val = setjmp(jmp_buffer);
        if (val) {
                Message("FATAL: %s:%s, %d Cannot continue, will exit!\n", __FILE__, __func__, __LINE__)
                exit(100);
        }

        // int parse_res = argp_parse(0, argc, argv, 0, 0, 0);

        FILE * json_file = fopen(dest, "w");
        if (! json_file) {
                Message("ERROR: %s,%d: Opening file %s: %s\n", __func__, __LINE__, filename, strerror(errno))
                longjmp(jmp_buffer, 100);
        }
        Message("INFO: Opened '%s' for reading (saving results to %s)\n", filename, dest)
        print_utmp(json_file);
        fclose(json_file);
        return (0);
}