#ifndef __JDUMPERTOOLS__

#define __JDUMPERTOOLS__

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <errno.h>
#include <setjmp.h>
#include <utmp.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/*
 * I don't want to use printf by accident. Poison it...
 */ 
#pragma GCC poison printf
#pragma pack(1)


#ifndef JDUMPERTOOLS_VERSION
#pragma GCC error "JDUMPERTOOLS_VERSION is missing, needs -DJDUMPERTOOLS_VERSION=$VERSION"
#endif

#define AUTHOR_EMAIL "<kodegeek.com@protonmail.com>"

const char *argp_program_version;
const char *argp_program_bug_address = AUTHOR_EMAIL;
const int IGNORE_YEAR = 1970;
const int BUFFER_SIZE = 1000;
const int LOG_SIZE = 1000;

#define Message(format, ...) fprintf(stderr, format, __VA_ARGS__);

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args);
#else
#define DEBUG_PRINT(fmt, args...)
#endif

jmp_buf jmp_buffer;

int disk_details(int num_paths, char **paths, FILE * json_file);
char * get_ut_type(const int ut_type);
bool utmpprint(const int idx, struct utmp *log, char *terminal, char *host, char * buffer, FILE * json_file);
int print_utmp(FILE * json_file);

#endif

