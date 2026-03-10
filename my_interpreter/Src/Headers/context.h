#ifndef REINCLUDE_CONTEXT_SENTRY
#define REINCLUDE_CONTEXT_SENTRY

#include "my_types.h"

context *create_and_init_context();
void destroy_context(context *cnt);

#endif /* !REINCLUDE_CONTEXT_SENTRY */