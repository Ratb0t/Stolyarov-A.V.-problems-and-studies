#include <stdio.h>

#define ALPHABET_LENGTH 'z' - 'a' + 1
#define LOWER_CASE 0x20

#define Exersize 2

#if Exersize == 1
int get_string_len(const char *str);
int is_letter(const char ch);
int get_longest_pram(int count, char **args);

int is_unique_letters_case_sensitive(const char *str);
int is_unique_letters(const char *str);

int get_params_with_unique_letters(int count, char **args, int (*predicate)(const char *));

int get_dots_and_dog(const char *str);
int get_params_dots_and_dog(int count, char **args);

int is_numeric(const char *str);
int get_numeric_params(int count, char **args);

int is_uniform_string(const char *str);
int get_uniform_param(int count, char **args);

int get_param_with_letter(int count, char **args);

int common_letter_with_first_arg(int count, char **args);

int main(int argc, char **argv)
{
    if(argc <= 1)
        return 0;    
    get_longest_pram(argc, argv);

    get_params_with_unique_letters(argc, argv, is_unique_letters_case_sensitive);

    get_params_dots_and_dog(argc, argv);

    get_numeric_params(argc, argv);
    
    get_uniform_param(argc, argv);

    get_param_with_letter(argc, argv);

    common_letter_with_first_arg(argc, argv);

    return 0;
}

int get_string_len(const char *str)
{
    int i;
    for(i = 0; str[i]; ++i);
    return i;
}
int is_letter(const char ch)
{
    if ((ch | LOWER_CASE) >= 'a' && (ch | LOWER_CASE) <= 'z')
        return 1;
    return 0;
}
int is_numeric(const char *str)
{
    for(int i = 0; str[i]; ++i)
        if(((unsigned int)(str[i] - '0')) > 9)
            return 0;
    return 1;
}
int is_uniform_string(const char *str)
{
    char prev_letter = 0;
    if(!*str)
        return 0;
    
    for(int i = 0; str[i]; ++i)
    {
        if(str[i] != prev_letter && prev_letter != 0)
            return 0;
        prev_letter = str[i];
    }
    

    return 1;
}

int get_longest_pram(int count, char **args)
{
    int length_max, current_length = 0;
    char *longest_param;
    for (int i = 1; i < count; ++i)
    {
        if ((current_length = get_string_len(args[i])) > length_max)
        {
            length_max = current_length;
            longest_param = args[i];
        }
    }
    if(*longest_param)
        printf("Longest: %s\nLength: %d\n\n", longest_param, length_max);

    return 1;
}

int get_params_with_unique_letters(int count, char **args, int (*predicate)(const char *))
{
    printf("Unique args: ");
    fflush(stdout);
    for(int i = 1; i < count; ++i)
    {
        if(predicate(args[i]) && args[i][0])
            printf("%s ", args[i]);
    }
    printf("\n\n");
    
    return 1;
}

int is_unique_letters_case_sensitive(const char *str)
{
    char alphabet[2][ALPHABET_LENGTH] = {0};
    int ch = 0;
    for(int i = 0; str[i]; ++i)
    {
        if (!is_letter(str[i]))
            continue;
        if (str[i] & LOWER_CASE)
        {
            ch = str[i] - 'a';
            if (alphabet[1][ch])
                return 0;
            alphabet[1][ch] += 1;
        }
        else
        {
            ch = str[i] - 'A';
            if (alphabet[0][ch])
                return 0;
            alphabet[0][ch] += 1;
        }
    }

    return 1;
}

int is_unique_letters(const char *str)
{
    char alphabet[ALPHABET_LENGTH] = {0};

    int ch;
    for(int i = 0; str[i]; ++i)
    {
        if(!is_letter(str[i]))
            continue;
        ch = (str[i] | LOWER_CASE) - 'a';
        if (alphabet[ch])
            return 0;
        alphabet[ch] += 1;
    }
    return 1;
}

int get_params_dots_and_dog(int count, char **args)
{
    printf("Single '@' many '.': ");
    fflush(stdout);
    for(int i = 1; i < count; ++i)
    {
        if (get_dots_and_dog(args[i]) && args[i][0])
            printf("%s ", args[i]);
    }
    printf("\n\n");
    return 1;
}

int get_dots_and_dog(const char *str)
{
    int dog = 0, dots = 0;
    for(int i = 0; str[i]; ++i)
    {
        if(str[i] == '@')
        {
            if(dog == 0) ++dog;
            else return 0;
        }
        if(str[i] == '.')
            ++dots;
    }

    return (dots > 1) && dog;
}

int get_numeric_params(int count, char **args)
{
    printf("Only numbers args: ");
    fflush(stdout);
    for(int i = 1; i < count; ++i)
        if(is_numeric(args[i]))
            printf("%s ", args[i]);
    printf("\n\n");
    return 1;
}

int get_uniform_param(int count, char **args)
{
    printf("Uniform args: ");
    fflush(stdout);
    for (int i = 1; i < count; ++i)
        if (is_uniform_string(args[i]))
            printf("%s ", args[i]);

    printf("\n\n");
    return 1;
}

int get_param_with_letter(int count, char **args)
{
    printf("Have one or more latin letter: ");
    fflush(stdout);
    for(int i = 1; i < count; ++i)
    {
        for(int j = 0; args[i][j]; ++j)
        {
            if(is_letter(args[i][j]))
                {printf("%s ", args[i]); break;}
        }
    }

    printf("\n\n");
    return 1;
}

int common_letter_with_first_arg(int count, char **args)
{
    char symbols[255] = {0};
    unsigned index;
    for(int i = 0; args[1][i]; ++i)
    {
        index = args[1][i] | LOWER_CASE;

        symbols[index] = 1;
    }
    printf("Have the same latter with the first args: ");
    fflush(stdout);
    for(int arg = 2; arg < count; ++arg)
    {
        for(int i = 0; args[arg][i]; ++i)
        {
            index = args[arg][i] | LOWER_CASE;
            if (symbols[index])
            {
                printf("%s ", args[arg]);
                break;
            }
        }
    }

    printf("\n\n");
    return 1;
}
#elif Exersize == 2
int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Wrong param count!\n");
        return 1;
    }

    int word_count = 0;
    int in_word = 0;

    for (int i = 0; argv[1][i]; i++)
    {
        if (argv[1][i] == ' ' || argv[1][i] == '\t')
            in_word = 0; 
        else
            if (!in_word)
            {
                word_count++;
                in_word = 1;
            }
    }
    printf("%d\n", word_count);
    return 0;
}

#endif //Exersize