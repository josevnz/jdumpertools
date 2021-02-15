#include <sys/statvfs.h>
#include <setjmp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include "jdumpertools.h"

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

char * get_ut_type(const int ut_type) {
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

bool utmpprint(const int idx, struct utmp *log, char *terminal, char *host, char * buffer, FILE * json_file) {

        if (log->ut_type == EMPTY || log->ut_type == ACCOUNTING) {
                return false;
        }

        time_t timestamp = log->ut_tv.tv_sec;
        struct tm * times = gmtime(&timestamp);
        const int record_year = 1900 + times->tm_year;
        if (record_year <= IGNORE_YEAR) {
                DEBUG_PRINT("Ignored year: %d\n", record_year);
                return false;
        }
        char * timestamp_str = ctime(&timestamp);

        memset(terminal, '\0', UT_LINESIZE + 1);
        strncpy(terminal, log->ut_line, UT_LINESIZE);

        memset(host, '\0', UT_HOSTSIZE + 1);
        strncpy(host, log->ut_host, UT_HOSTSIZE);

        struct in_addr ipnetw;
        ipnetw.s_addr = log->ut_addr_v6[0];
        char * address = inet_ntoa(ipnetw);
        char * utype = get_ut_type(log->ut_type);
        if (idx > 0) {
            fprintf(json_file, ",");
        }
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
        DEBUG_PRINT("buffer=%s\n", buffer)
        fprintf(json_file, buffer);
        return true;
}