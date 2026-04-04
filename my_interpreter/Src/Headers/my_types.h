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
    redirection_error,
    redirection_symbol_error,
    quotes_error,
    backslash_error,
    analyzartor_error_processed,
    alloc_error,
    start_dialog_error,
    delimiter_error,
    not_implemented,
    emty_input,
    clear_stdin,


};

enum lexeme_type
{
    default_word = 1,
    truncate_file,
    append_to_file,
    read_from_file,
    background_process,
};

typedef struct control_codes
{
    int major_code;
    union
    {
        struct bits
        {
            unsigned int fg_process : 1;
            unsigned int out_redirect : 2;
            unsigned int in_redirect : 1;
            unsigned int set_in_redirect_path : 1;
            unsigned int set_out_redirect_path : 1;
        } codes;
        unsigned int raw;

    } minore_code;
}control_codes;

typedef struct analyzator
{
    struct lexeme{
        my_string *word;
        enum lexeme_type type;
    } lexeme;
    my_list *words;
    int ch;
    int prev_char;
    int last_delimiter;
    int quotes;
    control_codes code;
} analyzator;

typedef struct process_handle
{
    /*int num_running_processes;
    int fg_pid;*/
    int need_init;
    my_string *input_redirection;
    my_string *output_redirection;
} process_handle;

typedef struct context
{
    analyzator *alzr;
    process_handle *proc_hanler;
    control_codes code;
} context;

#endif /* !REINCLUDE_MY_TYPES_SENTRY */