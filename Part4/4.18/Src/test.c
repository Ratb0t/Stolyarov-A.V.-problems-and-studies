#include "my_string.h"
#include <assert.h>
#include <stdio.h>
int main()
{
    //char control_result;
    //char *buf;
    struct my_string *str = create_string("Hello saivor\n");

    assert(str != 0);

    printf("%s", *(char **)str);

    assert(set_string(str, VERY_LONG_STRING) == 1);
    printf("%s", *(char **)str);

    destroy_my_string(str);

    return 0;
}