#ifndef REINCLUDE_GUARD_PROCESS_HANDLER_SENTRY
#define REINCLUDE_GUARD_PROCESS_HANDLER_SENTRY
#include "my_types.h"
#include "analyzator.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>


process_handle *create_process_handle();

void destroy_process_handle(process_handle *ph);

void init_process_handle(process_handle *ph);

/***************************************************************/
int format_cmd_line(analyzator *alzr, char *cmd_line[]);

/***************************************************************/
int process_cd_command(analyzator *alzr, char *path);

int start_external_prog(analyzator *alzr);
/***************************************************************/

int cleaning_background_processes(analyzator *alzr);

int wait_startetd_process_before_quite(analyzator *alzr);

int wait_foreground_process(analyzator *alzr);

#endif /* !REINCLUDE_GUARD_PROCESS_HANDLER_SENTRY */