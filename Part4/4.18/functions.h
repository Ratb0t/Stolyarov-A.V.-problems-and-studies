#ifndef REINCLUDE_PROTECTION_FUNCTIONS
#define REINCLUDE_PROTECTION_FUNCTIONS
#include <stdlib.h>
#include <stdio.h>

#define INT_CHAR_LENGTH 10

struct item
{
    long data;
    int times;
    struct item *next;
};

int print_3times_numbers();
int print_most_common_numbers();
int list_revers(struct item **lst);
int destroy_lst(struct item *lst);

//Use only on x64 platform 

extern int check_overflow(int a, int b);
int test_overflow();

#endif //! REINCLUDE_PROTECTION_FUNCTIONS