#ifndef REINCLUDE_PROTECTION_FUNCTIONS
#define REINCLUDE_PROTECTION_FUNCTIONS
#include <stdlib.h>
#include <stdio.h>
#include "my_string.h"
#include "my_vector.h"
#include "my_macroses.h"
#define DEBUG_PRINT_print_3times_numbers
#define DEBUG_PRINT_reverse_sentence1
struct item
{
    int data;
    int times;
};

/*
 * getline_int - По символьно читает из стандартного потока ввода 
 *               и преобразовывает к целочисленному знаковому значениию 
 *               (int)
 *      [out] int *out - заносится результат преобразования
 * 
 *      [out] int *readed - заносится количество считанных цифровых символов.
 *                         При возникновении переполнения результата заносится
 *                         отрицательные значения
 *      
 * Возвращаемые значения
 *      Возвращает не нулевое значение.
 *      При возникновении ошибки возвращает 0.
 *      При возникновении ситуации конец файла возвращает EOF
 */
int getline_int(int *out, int *readed);

int print_3times_numbers();
int print_most_common_numbers();

int reverse_sentence();
int print_coloumn_sentence();
int print_numbers_in_sentence();




//Use only on x64 platform 
extern int check_overflow(int a, int b);
int test_overflow();

#endif //! REINCLUDE_PROTECTION_FUNCTIONS