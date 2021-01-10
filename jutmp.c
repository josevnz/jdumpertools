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
#include <utmp.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <setjmp.h>
#include <argp.h>
#include <stdbool.h>

#include "jdumpertools.h"

jmp_buf jmp_buffer;

static const int BUFFER_SIZE = 1000;
static char *filename = "/var/log/wtmp";
static char *dest = "/tmp/jutmp.json";
static const int logsize = 1000;

char * get_ut_type(int ut_type) {
        if (ut_type == EMPTY)
                return "Unknown";
        if (ut_type == RUN_LVL)
                return "ChangeInRunLevel";
        if (ut_type == BOOT_TIME)
                return "BootTime";
        if (ut_type == NEW_TIME)
                return "TimeAfterClockChange";
        if (ut_type == OLD_TIME)
                return "TimeBeforeClockChange";
        if (ut_type == INIT_PROCESS)
                return "ProcSwapnedByInit";
        if (ut_type == LOGIN_PROCESS)
                return "SessionLeaderProcessForUserLogin";
        if (ut_type == USER_PROCESS)
                return "NormalProcess";
        if (ut_type == DEAD_PROCESS)
                return "TerminatedProcess";
        return "Accounting";
}

void utmpprint(struct utmp *log, char *terminal, char *host, char * buffer, FILE * json_file) {
        time_t timestamp = log->ut_tv.tv_sec;
        char * timestamp_str = ctime(&timestamp);

        memset(terminal, '\0', UT_LINESIZE + 1);
        strncpy(terminal, log->ut_line, UT_LINESIZE);

        memset(host, '\0', UT_HOSTSIZE + 1);
        strncpy(host, log->ut_host, UT_HOSTSIZE);

        struct in_addr ipnetw;
        ipnetw.s_addr = log->ut_addr_v6[0];
        char * address = inet_ntoa(ipnetw);
        char * utype = get_ut_type(log->ut_type);

        snprintf(
               buffer,
               BUFFER_SIZE,
               "{ \"timestamp\": \"%s\", \"user\": \"%s\", \"pid\": %d, \"tty\": \"%s\", \"session_id\": %d,\"type_of_record\": \"%s\", \"kernel\": \"%s\", \"exit\": {\"termination\": %i, \"exit code\": %i}, \"ip_addr\": \"%s\" }",
               strtok(timestamp_str, "\n\0"),
               log->ut_user,
               log->ut_pid,
               terminal,
               log->ut_session,
               utype,
               host,
               log->ut_exit.e_termination,
               log->ut_exit.e_exit,
               address);
        fprintf(json_file, buffer);
}

int print_utmp(FILE * json_file) {

        int file;
        struct utmp log[logsize];
        int i = 0;
        file = open(filename, O_RDONLY);
        if (file < 0) {
                Message("ERROR: %s:%d, Failed to open\n", __func__, __LINE__)
                longjmp(jmp_buffer, 100);
        }
        if (file) {
                read(file, &log, logsize * sizeof(struct utmp));
                char terminal[UT_LINESIZE + 1];
                char host[UT_HOSTSIZE + 1];
                char buffer[BUFFER_SIZE];

                fprintf(json_file, "[");
                for (i = 0; i < logsize; i++)
                {
                        utmpprint(&log[i], terminal, host, buffer, json_file);
                        if (i < logsize - 1)
                        {
                                fprintf(json_file, ",");
                        }
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