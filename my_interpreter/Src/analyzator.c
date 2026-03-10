#include "analyzator.h"

int get_analyzator_code(analyzator *alzr)
{
    if (alzr->ch == EOF)
        return quite;

    if (my_list_get_len(alzr->words))
        return exect_external; // print_input;

    if (alzr->quotes)
        alzr->code = quotes_error;

    analyzator_code_error_handler(alzr);
    return alzr->code;
}

void analyzator_code_error_handler(analyzator *alzr)
{
    switch (alzr->code)
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
    default:
        break;
    }
    alzr->code = analyzartor_error_processed;
    return;
}

analyzator *create_analyzator()
{
    analyzator *alzr = malloc(sizeof(analyzator));
    if(alzr == NULL)
        printf("Interpeter create error: Failed to allocate analyzator\n");
    return alzr;
}

void destroy_analyzator(analyzator *alzr) 
{
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
                alzr->code = alloc_error;
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
        alzr->code = backslash_error;
    }

    if (alzr->prev_char == '\\' && alzr->ch == '\\')
        return 0;

    return alzr->ch;
}

int process_symbol(analyzator *alzr)
{
    alzr->code = add_char;
    switch (alzr->ch)
    {
    case EOF:
        alzr->code = quite;
        break;
    case '\"':
        if (alzr->prev_char != '\\')
        {
            alzr->code = ok;
            alzr->quotes += 1;
        }
        break;
    case '\\':
        if (alzr->prev_char != '\\')
            alzr->code = ok;
        break;
    case '\n':
        alzr->code = end_input_line;
        insetr_word(alzr);
        break;
    case '\t':
    case ' ':
        if (!(alzr->quotes & 1))
            alzr->code = ok;
        insetr_word(alzr);
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
    alzr->code = ok;
    if (!(alzr->word = my_str_create(NULL)))
        alzr->code = alloc_error;
    if (!(alzr->words = my_list_create()))
    {
        alzr->code = alloc_error;
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


