#include <stdio.h>

#define PART 3


#if PART == 1

int main()
{
    int ch, prev = 10;
    int word_count = 0;
    int total_word_count = 0;
    int even_count = 0;
    int odd_count = 0;
    int word_letter_counter = 0;
    int seven_letter_words_count = 0;
    int two_letter_words_count = 0;
    int start_A = 0;
    int count_words_A_z = 0;
    int length_long_word = 0;
    int length_short_word = 0;
    int spaces_count = 0, length_spaces = 0;
    int bracket_balancer = 0;
    int empty_bracket_counter = 0;
    while ((ch = getchar()) != EOF)
    {
        if(ch != ' ' && ch != '\n')
            ++word_letter_counter;

        if(ch == 'A' && word_letter_counter == 1)
            start_A = 1;

        switch (ch)
        {
        case ' ':
        case '\n':
            {
                if(ch == ' ')++spaces_count;
                if(prev == '\n' || (prev == ch && ch == ' '))
                    continue;

                ++word_count;
                ++total_word_count;
                
                if(word_letter_counter & 1)
                    ++odd_count;
                else
                    ++even_count;

                if(word_letter_counter == 7)
                    ++seven_letter_words_count;
                
                if(word_letter_counter == 2)
                    ++two_letter_words_count;

                if(start_A && prev == 'z')
                    ++count_words_A_z;

                if(word_letter_counter > length_long_word)
                    length_long_word = word_letter_counter;
                
                if(word_letter_counter < length_short_word || 
                    length_short_word == 0)
                    length_short_word = word_letter_counter;

                if(ch == '\n')
                {
                    if(length_spaces < spaces_count)
                        length_spaces = spaces_count;

                    printf("String words count: %d\n"
                           "Total words input: %d\n"
                           "Words with an even number of characters: %d\n"
                           "Words with an odd number of characters: %d\n"
                           "Words consisting of seven letters: %d\n"
                           "Words consisting of two letters: %d\n"
                           "Words starting with 'A' and ending with 'z': %d\n"
                           "Length of the longest word in string: %d\n"
                           "Length of the shortest word in string: %d\n"
                           "The largest length of whitespace characters: %d\n"
                           "%s\n"
                           "Brackets that look like (): %d\n",
                           word_count, total_word_count, even_count,
                           odd_count, seven_letter_words_count,
                           two_letter_words_count, count_words_A_z,
                           length_long_word, length_short_word, length_spaces,
                           bracket_balancer == 0 ? "YES" : "NO",
                           empty_bracket_counter);
                    word_count = 0;
                    even_count = 0;
                    odd_count = 0;
                    seven_letter_words_count = 0;
                    two_letter_words_count = 0;
                    count_words_A_z = 0;
                    length_long_word = 0;
                    length_short_word = 0;
                    length_spaces = 0;
                    spaces_count = 0;
                    empty_bracket_counter = 0;
                    bracket_balancer = 0;
                }
                word_letter_counter = 0;
                start_A = 0;
            }
            break;
        case '(':
            ++bracket_balancer;
            break;
        case ')':
            if(prev == '(')
                ++empty_bracket_counter;
            -- bracket_balancer;
            break;
        }
        if(length_spaces < spaces_count)
        {
            length_spaces = spaces_count;
            spaces_count = 0;
        }
        prev = ch;
    }
    return 0;
}
#elif PART == 2
int main()
{
    int ch;
    int bracket = 0;
    int prev = 0;
    while((ch = getchar()) != EOF)
    {
        if (ch >= '0' && prev <= ' ')
        {
            putchar('(');
            ++bracket;
        }
        if(bracket && ch <= '0')
        {
            putchar(')');
            --bracket;
        }
        putchar(ch);
        prev = ch;
    }
    return 0;
}
#elif PART == 3
#define WORD_LENTH 2
int main()
{
    int ch;
    char letters[WORD_LENTH+1] = {0};
    int readed = 0;
    while ((ch = getchar()) != EOF)
    {
        if(ch <= ' ')
        {
            if (readed == WORD_LENTH)
                printf("%s ", letters);
            
            readed = 0;
            if(ch == '\n')putchar(ch);
            continue;
        }
#       if WORD_LENTH == 2

        letters[readed & 1] = ch;
#       else
        letters[readed % WORD_LENTH] = ch;
#       endif
        ++readed;
    }

    return 0;
}
#endif
