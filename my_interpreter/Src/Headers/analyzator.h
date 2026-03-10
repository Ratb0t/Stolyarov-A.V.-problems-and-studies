#ifndef REINCLUDE_GUARD_ANALYZATOR_SENTRY
#define REINCLUDE_GUARD_ANALYZATOR_SENTRY

#include <stdio.h>
#include "my_types.h"

analyzator *create_analyzator();

void destroy_analyzator(analyzator *alzr);

void init_analizator(analyzator *alzr);

void analyzator_code_error_handler(analyzator *alzr);
/********************************************************************/
int my_strcmp(const char *str1, const char *str2);

/********************************************************************/
int get_analyzator_code(analyzator *alzr);

void clear_stdin(int ch);

int process_symbol(analyzator *alzr);

/*******************************************************/

#endif /* !REINCLUDE_GUARD_ANALYZATOR_SENTRY */