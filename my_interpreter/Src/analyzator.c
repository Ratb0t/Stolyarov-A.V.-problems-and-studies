#include "analyzator.h"
#define PRINT_VARIANT_
static void analyzator_code_error_handler(analyzator *alzr);

void set_control_code(context *cnt)
{
    if (cnt->alzr->ch == EOF)
    {
        cnt->code.major_code = quite;
        return;
    }
    if (cnt->alzr->quotes)
        cnt->alzr->code.major_code = quotes_error;
    
    if (cnt->alzr->code.major_code != not_implemented)
    {
        analyzator_code_error_handler(cnt->alzr);
        if (my_list_get_len(cnt->alzr->words) && cnt->alzr->code.major_code == end_input_line)
#       ifdef PRINT_VARIANT
            cnt->alzr->code.major_code = print_input; //
#       else
            cnt->alzr->code.major_code = exect_external;  //
#       endif
        else
            cnt->alzr->code.major_code = emty_input;
    }
   
    cnt->code.minore_code = cnt->alzr->code.minore_code;
    cnt->code.major_code = cnt->alzr->code.major_code;
    return;
}

static void clear_list(my_list *lst)
{
    my_list_iterator it;
    while ((it = my_list_get_first(lst)))
    {
        my_str_destroy(it->data_holder.as_void);
        my_list_delete_item(lst, 0);
    }

    return;
}

static void analyzator_code_error_handler(analyzator *alzr)
{
    const char *analyzator_err_desc;
    switch (alzr->code.major_code)
    {
    case ok:
    case end_input_line:
        return;
        break;
    case quotes_error:
        analyzator_err_desc = "unmatched quotes\n";
        break;
    case alloc_error:
        analyzator_err_desc = "can't alloc memory\n";
        break;
    case backslash_error:
        analyzator_err_desc = "unknown escape sequence\n";
        break;
    case delimiter_error:
        if(alzr->ch != '\n')
            clear_stdin('\n');
        analyzator_err_desc = "wrong delimiter\n";
        break;
    case redirection_error:
        if (alzr->ch != '\n')
            clear_stdin('\n');
        analyzator_err_desc = "redirect error\n";
        break;
    case redirection_symbol_error:
        if (alzr->ch != '\n')
            clear_stdin('\n');
        analyzator_err_desc = "an unexpected character was encountered after redirection.\n";
        break;
        default:
        break;
    }
    printf("Analyzator error: %s", analyzator_err_desc);
    alzr->code.major_code = analyzartor_error_processed;
    //alzr->code.minore_code.codes.fg_process = 0;
    return;
}

analyzator *create_analyzator()
{
    analyzator *alzr = malloc(sizeof(analyzator));
    if(alzr == NULL)
        printf("Interpeter create error: Failed to allocate analyzator\n");
    return alzr;
}

int clear_analyzator(analyzator *alzr)
{
    if (alzr->words)
    {
        clear_list(alzr->words);
        my_list_destroy(alzr->words);
    }
    if (alzr->word)
        my_str_destroy(alzr->word);

    alzr->word = NULL;
    alzr->words = NULL;
    return 1;

}

void destroy_analyzator(analyzator *alzr) 
{
    clear_analyzator(alzr);
    free(alzr);
    return;
}

static int insert_word(context *cnt)
{
    analyzator *alzr = cnt->alzr;
    if (!(alzr->quotes & 1))
    {
        if (my_str_get_len(alzr->word) || alzr->quotes)
        {
            my_list_push_back(alzr->words, (top_type){.as_void = alzr->word});
            alzr->word = my_str_create(0);
            if (!alzr->word)
            {
                if (alzr->ch != '\n')
                    clear_stdin('\n');
                alzr->code.major_code = alloc_error;
                return quite;
            }
        }
        if (alzr->code.minore_code.codes.set_in_redirect_path)
        {
            cnt->proc_hanler->input_redirection = alzr->word;
            alzr->code.minore_code.codes.set_in_redirect_path = 0;
        }
        if (alzr->code.minore_code.codes.set_out_redirect_path)
        {
            cnt->proc_hanler->output_redirection = alzr->word;
            alzr->code.minore_code.codes.set_out_redirect_path = 0;
        }
        alzr->quotes = 0;
    }
    return ok;
}

static int escape_sequence_analysis(analyzator *alzr)
{
    // printf("prev_char == %d ch == %d\n", alzr->prev_char, alzr->ch);
    if (alzr->prev_char == '\\' && alzr->ch != '\\' && alzr->ch != '\"')
    {
        if (alzr->ch != '\n')
            clear_stdin('\n');
        alzr->code.major_code = backslash_error;
    }

    if (alzr->prev_char == '\\' && alzr->ch == '\\')
        return '\n'; /*\n не может быть предыдущим символом т.к. является конечным*/
    
    if(alzr->ch == ' ')
        return alzr->prev_char;
        
    return alzr->ch;
}

static int is_word_after_redirect(analyzator *alzr)
{
    switch (alzr->ch)
    {
    case '>':
        if (alzr->prev_char == '>' && alzr->code.minore_code.codes.out_redirect != redirec_err)
            return 1;
    case ';':
    case '|':
    case '(':
    case ')':
    case '<':
    case '&':
        //if(alzr->prev_char == 0)
            //return 0;
        return 0;
        break;
    default:
        break;
    }
    if((alzr->prev_char == '>' || alzr->prev_char == '<') && alzr->ch == '\n')
        return 0;
    
    return 1;
}

int delimiter_analyse(analyzator *alzr)
{
    static int need_check_redirect = 0;
    if ((alzr->quotes & 1))
        return ok;
    if (need_check_redirect && !is_word_after_redirect(alzr))
    {
        alzr->code.major_code = redirection_symbol_error;
        return quite;
    }
    need_check_redirect = 0;
    switch (alzr->ch)
    {
    case '&':
        if(alzr->prev_char == 0)
        {
            alzr->code.major_code = delimiter_error;
            return quite;
        }
        /*
        if (alzr->prev_char == '&') => &&
        {
            сделать то, что означает &&
        }
        else
        {
            сделать то, что означает &
        }
        break;
        */
        if (alzr->prev_char != '&')
        {
            alzr->code.major_code = ok;
            alzr->code.minore_code.codes.fg_process = 0;
            break; /*т.к. не реализован '&&' использую fall-through*/
        }
    case '>':
        alzr->code.minore_code.codes.out_redirect += 1;
        alzr->code.major_code = ok;
        
        need_check_redirect = 1;
        if (alzr->code.minore_code.codes.out_redirect == truncate_file)
        {
            alzr->code.minore_code.codes.set_out_redirect_path = 1;
            break;
        }
        if (alzr->prev_char == '>' && alzr->last_delimiter == '>' && 
            alzr->code.minore_code.codes.out_redirect == append_to_file)
        {
            break;
        }
        alzr->code.major_code = redirection_error;
        alzr->code.minore_code.codes.set_out_redirect_path = 0;
        need_check_redirect = 0;
        return quite;
        break;
    case '<':
        if(alzr->code.minore_code.codes.in_redirect == 0)
        {
            alzr->code.minore_code.codes.in_redirect = 1;
            alzr->code.minore_code.codes.set_in_redirect_path = 1;
            alzr->code.major_code = ok;
            need_check_redirect = 1;
            break;
        }
        alzr->code.minore_code.codes.set_in_redirect_path = 0;
        alzr->code.major_code = redirection_error;
        need_check_redirect = 0;
        return quite;
        break;
    case '|':
        /*if (alzr->prev_char == '|')
            "Сделать что-то если ||";*/
    case '(':
    case ')':
    case ';':
        alzr->code.major_code = not_implemented;
        if(alzr->ch != '\n')
            clear_stdin('\n');
        break;
    default:
        if (alzr->ch != ' ' && alzr->ch != '\n')
        {
            if (alzr->last_delimiter == '&')
            {
                alzr->code.major_code = delimiter_error;
                alzr->code.minore_code.codes.fg_process = 1;
            }
        }
        return ok;
        break;
    }
    alzr->last_delimiter = alzr->ch;
    return ok;
}

int process_symbol(context *cnt)
{
    analyzator *alzr = cnt->alzr;
    alzr->code.major_code = add_char;
    if(alzr->ch != ' ' && delimiter_analyse(alzr) != ok) return quite;
    switch (alzr->ch)
    {
    case EOF:
        alzr->code.major_code = quite;
        return quite;
        break;
    case '\"':
        if (alzr->prev_char != '\\')
        {
            alzr->code.major_code = ok;
            alzr->quotes += 1;
        }
        break;
    case '\\':
        if (alzr->prev_char != '\\')
            alzr->code.major_code = ok;
        break;
    case '\n':
        alzr->code.major_code = end_input_line;
        insert_word(cnt);
        break;
    case '>':
        if (alzr->code.minore_code.codes.out_redirect == truncate_file)
        {
            if (!(alzr->quotes & 1))
                alzr->code.major_code = ok;
            insert_word(cnt);
        }
        break;
    case '\t':
    case '<':
    case ' ':
        if (!(alzr->quotes & 1))
            alzr->code.major_code = ok;
        insert_word(cnt);
        break;
    default:
        break;
    }

    alzr->prev_char = escape_sequence_analysis(alzr);
    return ok;
}

void init_analizator(analyzator *alzr)
{
    alzr->ch = 0;
    alzr->prev_char = 0;
    alzr->quotes = 0;
    alzr->last_delimiter = 0;
    alzr->code.major_code = ok;
    alzr->code.minore_code.raw = 0;
    alzr->code.minore_code.codes.fg_process = 1;
    alzr->word = NULL;
    alzr->words = NULL;
    return;
}

void reset_analizator(analyzator *alzr)
{
    alzr->ch = 0;
    alzr->prev_char = 0;
    alzr->quotes = 0;
    alzr->last_delimiter = 0;
    alzr->code.major_code = ok;
    alzr->code.minore_code.raw = 0;
    alzr->code.minore_code.codes.fg_process = 1;

    if (!(alzr->word = my_str_create(NULL)))
        alzr->code.major_code = alloc_error;

    if (!(alzr->words = my_list_create()))
    {
        alzr->code.major_code = alloc_error;
        my_str_destroy(alzr->word);
        alzr->word = NULL;
    }
    analyzator_code_error_handler(alzr);
    return;
}

int my_strcmp(const char *str1, const char *str2)
{
    int i = 0;
    for (; str1[i] && str2[i]; ++i)
    {
        if (str1[i] != str2[i])
            break;
    }
    if (str1[i] == str2[i])
        return compare_equal;
    if (str1[i] > str2[i])
        return compare_great;

    return compare_less;
}

void clear_stdin(int ch)
{

    while (getchar() != ch)
    {
    }
    return;
}

