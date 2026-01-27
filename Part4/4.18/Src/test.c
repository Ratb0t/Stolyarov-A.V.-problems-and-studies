#include "my_string.h"
#include <assert.h>
#include <stdio.h>
int main()
{
    //char control_result;
    //char *buf;
    struct my_string *str1 = my_str_create("Hello saivor\n");
    struct my_string *str2 = my_str_create(" world ");

    assert(str1 != 0);

    printf("%s\n", *(char **)str1);

    //assert(set_string(str1, VERY_LONG_STRING) == 1);
    printf("%s\n", *(char **)str2);

    struct my_string *result1 = my_str_insert(str1, 5, str2);
    struct my_string *result2 = my_str_concatenation(str1, str2);
    my_str_pushback_char(result2, 's');
    my_str_pushback_char(result2, '\n');
    printf("%s\n", my_str_get_data(result1));
    printf("%s\n", my_str_get_data(result2));


    my_str_destroy(str1);
    my_str_destroy(str2);
    my_str_destroy(result1);
    my_str_destroy(result2);

    return 0;
}