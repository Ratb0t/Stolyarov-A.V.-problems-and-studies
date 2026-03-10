#ifndef REINCLUDE_GUARD_ANALYZATOR_SENTRY
#define REINCLUDE_GUARD_ANALYZATOR_SENTRY

#include <stdio.h>
#include "my_types.h"

analyzator *create_analyzator();

void destroy_analyzator(analyzator *alzr);

int my_strcmp(const char *str1, const char *str2);

void init_analizator(analyzator *alzr);

void set_analyzator_code(analyzator *alzr);

void clear_stdin(int ch);

/*******************************************************/

#endif /* !REINCLUDE_GUARD_ANALYZATOR_SENTRY */