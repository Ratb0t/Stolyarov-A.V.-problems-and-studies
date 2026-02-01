
#ifndef REINCLUDE_PROTECTION_MY_STRING
#define REINCLUDE_PROTECTION_MY_STRING

#include <stdlib.h>
#include <stdint.h>
#include "my_macroses.h"
#define CAPACITY_FACTOR 1.5
#define STRING_RESIZE_THRESHOLD 70
#define PERCENT_OF(n1, n2)(n1) * 100 / (n2)
#ifdef DEBUG
#define LONG_STRING_FOR_TESTS
#endif

struct my_string;

int my_memcpy(void *dest, const void *src, size_t n);
int my_strlen(const char *buf);
int my_str_get_len(const struct my_string *str);
const char *my_str_get_data(const struct my_string *str);

struct my_string *my_str_create(const char *buf);
struct my_string *my_str_create_with_capacity(int capacity);

int my_str_pushback_char(struct my_string *str, char ch);
struct my_string *my_str_insert(const struct my_string *dest, int pos, const struct my_string *src);
struct my_string *my_str_concatenation(const struct my_string *str1, const struct my_string *str2);

int my_str_set_string(struct my_string *str, const char *buf);

int my_str_destroy(struct my_string *str);

#ifndef LONG_STRING_FOR_TESTS
#define DEFAULT_CAPACITY_SIZE 256
#define VERY_LONG_STRING "Hello world!!!\n"
#else
#define DEFAULT_CAPACITY_SIZE 5
#include <stdio.h>
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