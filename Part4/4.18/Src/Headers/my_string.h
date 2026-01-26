
#ifndef REINCLUDE_PROTECTION_MY_STRING
#define REINCLUDE_PROTECTION_MY_STRING

#include <stdlib.h>

#define CAPACITY_FACTOR 1.5
#define LONG_STRING_FOR_TESTS1

struct my_string;

int my_strlen(const char *buf);
int get_my_string_len(struct my_string *str);

int set_string(struct my_string *str, const char *buf);
struct my_string *create_string(const char *buf);

int destroy_my_string(struct my_string *str);


#ifndef LONG_STRING_FOR_TESTS
#define VERY_LONG_STRING "Hello world!!!\n"
#else 
#define VERY_LONG_STRING "The Valgrind distribution currently"\
"includes seven production-quality tools: a memory error "\
"detector, two thread error detectors, a cache and "\
"branch-prediction profiler, a call-graph generating cache "\
"and branch-prediction profiler, and two different heap "\
"profilers. It also includes an experimental SimPoint basic "\
"block vector generator. It runs on the following platforms: "\
"X86/Linux, AMD64/Linux, ARM32/Linux, ARM64/Linux, PPC32/Linux, "\
"PPC64BE/Linux, PPC64LE/Linux, S390X/Linux, MIPS32/Linux, "\
"MIPS64/Linux, RISCV64/Linux, X86/Solaris, AMD64/Solaris, "\
"ARM/Android (2.3.x and later), ARM64/Android, X86/Android "\
"(4.0 and later), MIPS32/Android, X86/FreeBSD, AMD64/FreeBSD, "\
"ARM64/FreeBSD, X86/Darwin and AMD64/Darwin (Mac OS X 10.12).\n"
#endif // !LONG_STRING_FOR_TESTS

#endif //! REINCLUDE_PROTECTION_MY_STRING