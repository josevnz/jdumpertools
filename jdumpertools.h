#ifndef __JDUMPERTOOLS__

#define __JDUMPERTOOLS__
#pragma GCC poison printf
#pragma pack(1)

#include <jansson.h>

#ifndef JDUMPERTOOLS_VERSION
#pragma GCC error "JDUMPERTOOLS_VERSION is missing, needs -DJDUMPERTOOLS_VERSION=$VERSION"
#endif

#define AUTHOR_EMAIL "<kodegeek.com@protonmail.com>"

const char *argp_program_version;
const char *argp_program_bug_address = AUTHOR_EMAIL;

#define Message(format, ...) fprintf(stderr, format, __VA_ARGS__);

#endif

