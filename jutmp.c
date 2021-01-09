/*
 * Simple program to read utmp log and create a nice JSON document from it.
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

void utmpprint(struct utmp *log, char *terminal, char *host, char * buffer) {
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
}

int main() {

        int val = setjmp(jmp_buffer);
        if (val){
                fprintf(stderr, "FATAL: Cannot continue, will exit!\n");
                exit(100);
        }

        int file;
        struct utmp log[logsize];
        int i = 0;
        file = open(filename, O_RDONLY);
        if (file < 0) {
                fprintf(stderr, "ERROR Failed to open");
                longjmp(jmp_buffer, 100);
        }
        if (file) {
                read(file, &log, logsize * sizeof(struct utmp));
                char terminal[UT_LINESIZE + 1];
                char host[UT_HOSTSIZE + 1];
                char buffer[BUFFER_SIZE];

                FILE * json_file = fopen(dest, "w");
                if (! json_file) {
                        fprintf(stderr, "ERROR: Opening file %s: %s", filename, strerror(errno));
                        longjmp(jmp_buffer, 100);
                }
                fprintf(stderr, "INFO: Opened '%s' for reading (saving results to %s)\n", filename, dest);

                fprintf(json_file, "[");
                for (i = 0; i < logsize; i++)
                {
                        utmpprint(&log[i], terminal, host, buffer);
                        fprintf(json_file, buffer);
                        if (i < logsize - 1)
                        {
                                fprintf(json_file, ",");
                        }
                }
                fprintf(json_file, "]\n");
                fclose(json_file);
                close(file);
        } else {
                fprintf(stderr, "ERROR: No UTMP file!\n");
                longjmp(jmp_buffer, 100);
        }
        return (0);
}