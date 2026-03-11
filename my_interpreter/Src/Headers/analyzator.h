#ifndef REINCLUDE_GUARD_ANALYZATOR_SENTRY
#define REINCLUDE_GUARD_ANALYZATOR_SENTRY

#include <stdio.h>
#include "my_types.h"
#include "my_list.h"

void analyzator_code_error_handler(analyzator *alzr);

/********************************************************************/
analyzator *create_analyzator();

int clear_analyzator(analyzator *alzr);

void destroy_analyzator(analyzator *alzr);

void init_analizator(analyzator *alzr);

void reset_analizator(analyzator *alzr);

/********************************************************************/
int my_strcmp(const char *str1, const char *str2);

/********************************************************************/
void set_control_code(context *cnt);

void clear_stdin(int ch);

int process_symbol(analyzator *alzr);

/*******************************************************/

#endif /* !REINCLUDE_GUARD_ANALYZATOR_SENTRY */