#include "analyzator.h"

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
            cnt->alzr->code.major_code = exect_external; // cnt->code.major_code = print_input; //
        else
            cnt->alzr->code.major_code = emty_input;
    }
   
    cnt->code.minore_code = cnt->alzr->code.minore_code;
    cnt->code.major_code = cnt->alzr->code.major_code;
    return;
}

void clear_list(my_list *lst)
{
    my_list_iterator it;
    while ((it = my_list_get_first(lst)))
    {
        my_str_destroy(it->data_holder.as_void);
        my_list_delete_item(lst, 0);
    }

    return;
}

void analyzator_code_error_handler(analyzator *alzr)
{
    
    switch (alzr->code.major_code)
    {
    case ok:
    case end_input_line:
        return;
        break;
    case quotes_error:
        printf("Analyzator error: unmatched quotes\n");
        break;
    case alloc_error:
        printf("Analyzator error: can't alloc memory\n");
        break;
    case backslash_error:
        printf("Analyzator error: unknown escape sequence\n");
        break;
    case delimiter_error:
        if(alzr->ch != '\n')
            clear_stdin('\n');
        printf("Analyzator error: wrong delimiter\n");
        break;
    default:
        break;
    }
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

int insetr_word(analyzator *alzr)
{
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
        alzr->quotes = 0;
    }
    return ok;
}

int escape_sequence_analysis(analyzator *alzr)
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

    return alzr->ch;
}

int delimiter_analyse(analyzator *alzr)
{
    if ((alzr->quotes & 1))
        return alzr->ch;
    switch (alzr->ch)
    {
    case '&':
        if(alzr->prev_char == 0)
        {
            alzr->code.major_code = delimiter_error;
            break;
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
        /*if (alzr->prev_char == '>')
            "Сделать что-то если >>";*/
    case '<':
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
        return alzr->ch;
        break;
    }
    alzr->last_delimiter = alzr->ch;
    return alzr->ch;
}

int process_symbol(analyzator *alzr)
{
    alzr->code.major_code = add_char;
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
        insetr_word(alzr);
        break;
    case '\t':
    case ' ':
        if (!(alzr->quotes & 1))
            alzr->code.major_code = ok;
        insetr_word(alzr);
        break;
    default:
        break;
    }

    delimiter_analyse(alzr);
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

