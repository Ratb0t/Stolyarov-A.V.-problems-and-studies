#ifndef REINCLUDE_GUARD_MAIN_SENTRY
#define REINCLUDE_GUARD_MAIN_SENTRY

#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "my_list.h"
#include "my_types.h"

#include "context.h"

void out_input(my_list *lst);

/********************************************************************/
int start_dialog();

void dialog(context *cnt);

int end_dialog(context *cnt);
/********************************************************************/

int dialog_codes_process(context *cnt);

int dispatch_dialog_error(context *cnt);

#endif /* !REINCLUDE_GUARD_MAIN_SENTRY */