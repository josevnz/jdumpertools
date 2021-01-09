#ifndef __JDUMPERTOOLS__

#define __JDUMPERTOOLS__
#pragma GCC poison printf
#pragma pack(1)

#ifndef JDUMPERTOOLS_VERSION
#pragma GCC error "JDUMPERTOOLS_VERSION is missing, needs -DJDUMPERTOOLS_VERSION=$VERSION"
#endif

const char *argp_program_version;
const char *argp_program_bug_address = "<kodegeek.com@protonmail.com>";

#include <jansson.h>

#endif

