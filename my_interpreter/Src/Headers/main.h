#ifndef REINCLUDE_GUARD_MAIN_SENTRY
#define REINCLUDE_GUARD_MAIN_SENTRY

#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "my_list.h"
#include "my_types.h"

#include "analyzator.h"
#include "process_handler.h"

void out_input(my_list *lst);

int start_dialog();

void dialog(analyzator *alzr);

int end_dialog(analyzator *alzr);

int dialog_codes_process(analyzator *alzr);

int dispatch_dialog_error(analyzator *alzr);

#endif /* !REINCLUDE_GUARD_MAIN_SENTRY */