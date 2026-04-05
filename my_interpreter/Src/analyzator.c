#include "analyzator.h"
#define _PRINT_VARIANT
#define MAX_DELIMITER_LEN 2

typedef int (*delimiter_function)(analyzator *alzr);

static struct lexeme *create_lexeme();
static int destroy_lexeme(void *lexeme);
static int init_lexeme(struct lexeme *lex);

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
        if (my_list_get_len(cnt->alzr->lexemes) && cnt->alzr->code.major_code == end_input_line)
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

static void clear_list(my_list *lst, int (*func)(void *))
{
    my_list_iterator it;
    while ((it = my_list_get_first(lst)))
    {
        if(func != NULL)
            func(it->data_holder.as_void);
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
    case add_char:
        return;
        break;
    case quotes_error:
        analyzator_err_desc = "unmatched quotes.";
        break;
    case alloc_error:
        analyzator_err_desc = "can't alloc memory.";
        break;
    case backslash_error:
        analyzator_err_desc = "unknown escape sequence.";
        break;
    case delimiter_error:
        analyzator_err_desc = "wrong delimiter.";
        break;
    case redirection_error:
        analyzator_err_desc = "redirect error.";
        break;
    case redirection_symbol_error:
        analyzator_err_desc = "an unexpected character was encountered after redirection.";
        break;
    default:
        return;
        break;
    }
    printf("Analyzator error: %s\n", analyzator_err_desc);
    alzr->code.major_code = clear_stdin;
    //alzr->code.minore_code.codes.fg_process = 0;
    return;
}

static struct lexeme *create_lexeme()
{
    struct lexeme *l = malloc(sizeof(struct lexeme));
    return l;
}
static int destroy_lexeme(void *lexeme)
{
    struct lexeme *lex = lexeme;
    if(lex->word != NULL)
        my_str_destroy(lex->word);
    free(lex);
    return 1;
}
static int init_lexeme(struct lexeme *lex)
{
    lex->word = my_str_create(NULL);
    if(lex->word == NULL)
        return alloc_error;
    lex->type = default_word;
    return ok;    
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
    if (alzr->lexemes)
    {
        clear_list(alzr->lexemes, destroy_lexeme);
        my_list_destroy(alzr->lexemes);
    }
    if (alzr->lexeme)
        destroy_lexeme(alzr->lexeme);

    alzr->lexeme = NULL;
    alzr->lexemes = NULL;
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
        if (my_str_get_len(alzr->lexeme->word) || alzr->quotes)
        {
            my_list_push_back(alzr->lexemes, (top_type){.as_void = alzr->lexeme});
            alzr->lexeme = create_lexeme();
            if (alzr->lexeme == NULL)
            {
                alzr->code.major_code = alloc_error;
                return quite;
            }
            if(init_lexeme(alzr->lexeme) == alloc_error)
            {
                alzr->code.major_code = alloc_error;
                return quite;
            }
        }
#       ifdef legacy
        if (alzr->code.minore_code.codes.set_in_redirect_path)
        {
            cnt->proc_hanler->input_redirection = alzr->lexeme->word;
            alzr->code.minore_code.codes.set_in_redirect_path = 0;
        }
        if (alzr->code.minore_code.codes.set_out_redirect_path)
        {
            cnt->proc_hanler->output_redirection = alzr->lexeme->word;
            alzr->code.minore_code.codes.set_out_redirect_path = 0;
        }
#       endif
        alzr->quotes = 0;
    }
    return ok;
}

static int escape_sequence_analysis(analyzator *alzr)
{
    // printf("prev_char == %d ch == %d\n", alzr->prev_char, alzr->ch);
    if (alzr->prev_char == '\\' && alzr->ch != '\\' && alzr->ch != '\"')
    {
        alzr->code.major_code = backslash_error;
    }

    if (alzr->prev_char == '\\' && alzr->ch == '\\')
        return '\n'; /*\n не может быть предыдущим символом т.к. является конечным*/
    
    if(alzr->ch == ' ')
        return alzr->prev_char;
        
    return alzr->ch;
}
#ifdef legacy
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

int _delimiter_analyse(analyzator *alzr)
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
#endif

static int set_delimiter_type(context *cnt, int delimiter_len)
{
    analyzator *alzr = cnt->alzr;
    switch (alzr->ch)
    {

    case '<':
        alzr->lexeme->type = read_from_file;
        if(delimiter_len)
            alzr->code.major_code = delimiter_error;
        break;
    case '(':
    case ')':
    /*У скобок не должно быть delimiter_error т.к.
    ситуация (( и )) возможна*/
        alzr->code.major_code = not_implemented;
        /*alzr->lexeme.type = not_implemented;*/
        break;
    case ';':
        /*alzr->lexeme.type = not_implemented;*/
        alzr->code.major_code = not_implemented;
        if (delimiter_len == 1)
        {
                alzr->code.major_code = delimiter_error;
        }
        break;
    case '>':
        /*
        cat main.c >> my_file &
        cat main.c>>my_file&
        cat main.c>>my_file &
        cat main.c>> my_file&
        cat main.c >> my_file&
        */
        alzr->lexeme->type = truncate_file;
        if (delimiter_len == 1)
        {
            if (my_str_get_data(alzr->lexeme->word)[0] == alzr->ch)
                alzr->lexeme->type = append_to_file;
            else
                alzr->code.major_code = delimiter_error;
        }
        break;
    case '&':
        alzr->lexeme->type = background_process;
        if (delimiter_len == 1)
        {
            if (my_str_get_data(alzr->lexeme->word)[0] == alzr->ch)
            {
                /*alzr->lexeme.type = not_implemented;*/
                alzr->code.major_code = not_implemented;
            }
            else
                alzr->code.major_code = delimiter_error;
        }
        break;
    case '|':
        alzr->code.major_code = not_implemented;
        /*alzr->lexeme.type = not_implemented;*/
        if (delimiter_len == 1)
        {
            if (my_str_get_data(alzr->lexeme->word)[0] == alzr->ch)
            {
                /*alzr->lexeme.type = not_implemented;*/
                alzr->code.major_code = not_implemented;
            }
            else
                alzr->code.major_code = delimiter_error;
        }
        break;
    default:
        return ok;
        break;
    }

    return quite;
}

static int delimiter_analyse(context *cnt)
{
    analyzator *alzr = cnt->alzr;
    int delimiter_len = 0;
    if(cnt->alzr->quotes&1)
        return ok;
    /* 
    Сейчас сгруппированы по реализации, чтобы
    лишний раз не писать не реализовано

    После реализации каждого разделителя
    сгруппировать может ли разделитель состоять
    из двух символов (&&, >>, ||) или нет
    */
    switch (alzr->ch)
    {

    case '<':
    
    case '(':
    case ')':
    case ';':

    
    case '>':
        /*
        cat main.c >> my_file &
        cat main.c>>my_file&
        cat main.c>>my_file &
        cat main.c>> my_file&
        cat main.c >> my_file&
        */    
    case '&':
    case '|':
        {
            if (alzr->lexeme->type == default_word)
                 if(!insert_word(cnt)) return quite;

            delimiter_len = my_str_get_len(alzr->lexeme->word);
            if (delimiter_len >= MAX_DELIMITER_LEN)
            {
                alzr->code.major_code = delimiter_error;
            }
        }
        break;
    default:
        return ok;
        break;
    }

    set_delimiter_type(cnt, delimiter_len);
    analyzator_code_error_handler(cnt->alzr);
    return quite;
}

int process_symbol(context *cnt)
{
    analyzator *alzr = cnt->alzr;

    alzr->code.major_code = add_char;
    /*if(alzr->ch != ' ' && delimiter_analyse(alzr) != ok) return quite;*/
    if(!delimiter_analyse(cnt)) return ok;

    switch (alzr->ch)
    {
    case EOF:
        alzr->code.major_code = end_input_line;
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
        if (alzr->lexeme->type != default_word)
            insert_word(cnt);
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
    alzr->lexeme = NULL;
    alzr->lexemes = NULL;
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

    alzr->lexeme = create_lexeme();
    if(alzr->lexeme == NULL)
        alzr->code.major_code = alloc_error;

    if (init_lexeme(alzr->lexeme) == alloc_error)
    {
        alzr->code.major_code = alloc_error;
        destroy_lexeme(alzr->lexeme);
        alzr->lexeme = NULL;
        return;
    }

    if (!(alzr->lexemes = my_list_create()))
    {
        alzr->code.major_code = alloc_error;
        destroy_lexeme(alzr->lexeme);
        alzr->lexeme = NULL;
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

int flush_stdin(context *cnt)
{
    while (cnt->alzr->ch != '\n' && cnt->alzr->ch != EOF)
    {
        cnt->alzr->ch = getchar();
    }
    return 1;
}