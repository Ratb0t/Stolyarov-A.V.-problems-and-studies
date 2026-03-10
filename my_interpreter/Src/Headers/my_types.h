#ifndef REINCLUDE_MY_TYPES_SENTRY
#define REINCLUDE_MY_TYPES_SENTRY
#include "my_string.h"
#include "my_list.h"

enum codes
{
    quite,
    ok,
    add_char,
    end_input_line,
    print_input,
    exect_external,
    fork_error,
    home_env_error,
    quotes_error,
    alloc_error,
    backslash_error,
    start_dialog_error
};

typedef struct analyzator
{
    my_string *word;
    my_list *words;
    int ch;
    int prev_char;
    int quotes;
    int code;
} analyzator;

typedef struct process_handle
{
    int num_running_processes;
    int fg_pid;
    int need_init;
} process_handle;

typedef struct context
{
    analyzator *alzr;
    process_handle *proc_hanler;
} context;

#endif /* !REINCLUDE_MY_TYPES_SENTRY */