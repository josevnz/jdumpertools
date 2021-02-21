/*
 * Functions used across the jdumpertools programs.
 * author: Jose Vicente Nunez
 */
#include "jdumpertools.h"
#include <sys/statvfs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

static const int TIMESTAMP_SIZE = 100;

int disk_details(int num_paths, char **paths, FILE * dest_file, int start_idx) {
    struct statvfs results;
    DEBUG_PRINT("num_paths=%d\n", num_paths)
    fprintf(dest_file, "[");
    for(int i =start_idx; i < num_paths; i++) {
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
            longjmp(jmp_buffer, 100);
        }
        double free_space = (double) results.f_bavail * results.f_frsize;
        double total_space = (double) results.f_blocks * results.f_frsize;
        fprintf(dest_file, "{\"partition\": \"%s\", \"free_space\": %f, \"total_space\": %f}", path, free_space, total_space);
        if (i< num_paths -1) {
            fprintf(dest_file, ",");
        }
    } // end loop
    fprintf(dest_file, "]\n");
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

bool utmpprint(const int idx, struct utmp *log, char *terminal, char *host, FILE * dest_file) {

        if (log->ut_type == EMPTY || log->ut_type == ACCOUNTING) {
                return false;
        }

        time_t timestamp = log->ut_tv.tv_sec;
        struct tm * times = localtime(&timestamp);
        const int record_year = 1900 + times->tm_year;
        if (record_year <= IGNORE_YEAR) {
                DEBUG_PRINT("Ignored year: %d\n", record_year);
                return false;
        }
        char timestamp_str[TIMESTAMP_SIZE];
        strftime(timestamp_str, TIMESTAMP_SIZE,"%c", times);

        memset(terminal, '\0', UT_LINESIZE + 1);
        strncpy(terminal, log->ut_line, UT_LINESIZE);

        memset(host, '\0', UT_HOSTSIZE + 1);
        strncpy(host, log->ut_host, UT_HOSTSIZE);

        struct in_addr ipnetw;
        ipnetw.s_addr = log->ut_addr_v6[0];
        char * address = inet_ntoa(ipnetw);
        char * utype = get_ut_type(log->ut_type);
        if (idx > 0) {
            fprintf(dest_file, ",");
        }
        fprintf(dest_file, "{ \"timestamp\": \"%s\", \"user\": \"%s\", \"pid\": %d, \"tty\": \"%s\", \"session_id\": %d,\"type_of_record\": \"%s\", \"kernel\": \"%s\", \"exit\": {\"termination\": %i, \"exit code\": %i}, \"ip_addr\": \"%s\" }",
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
        return true;
}

int print_utmp(FILE * dest_file) {

        int file;
        struct utmp log[LOG_SIZE];
        int i = 0;
        DEBUG_PRINT("filename=%s\n", filename)
        file = open(WTMP_FILE, O_RDONLY);
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

                fprintf(dest_file, "[");
                for (i = 0; i < LOG_SIZE; i++) {
                        utmpprint(i, &log[i], terminal, host, dest_file);
                }
                fprintf(dest_file, "]\n");
                close(file);
        } else {
                Message("ERROR: %s, %d: No UTMP file!\n", __func__, __LINE__)
                longjmp(jmp_buffer, 100);
        }
        return 0;
}

void alarmHandler(const int signal) {
        Message("ERROR: %s, %d: Program took too long to finish (SIGNAL=%d)!\n", __func__, __LINE__, signal)
        abort();
}

void ignoreNohupHandler(const int signal) {
        Message("INFO: %s, %d: Ignoring NOHUP (SIGNAL=%d)!\n", __func__, __LINE__, signal)
}