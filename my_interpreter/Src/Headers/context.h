#ifndef REINCLUDE_CONTEXT_SENTRY
#define REINCLUDE_CONTEXT_SENTRY

#include <stdlib.h>
#include "my_types.h"
#include "analyzator.h"
#include "process_handler.h"

context *create_and_init_context();
void destroy_context(context *cnt);

void context_reset_analizator(context *cnt);

#endif /* !REINCLUDE_CONTEXT_SENTRY */