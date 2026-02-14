extern int print_int(int val);
extern int print(const char* str);
extern int getchar();
extern int putchar(int c);
extern void *mmap(int page_count);
extern int munmap(void* maped_address, int page_count);

#define NULL (void *)0
#define EOF -1
typedef struct stack
{
    void *stack;
    void *head;
    int size;
    int type_size;
} stack;
void init_simple_stack(stack *st, void *buf, int buf_size, int size_type)
{
    st->size = buf_size;
    st->type_size = size_type;
    st->stack = buf;
    st->head = (unsigned char *)buf - size_type;
    return;
}

void *get_element(stack *st)
{
    if (((char *)st->head - (char *)st->stack) < 0)
        return NULL;

    return st->head;
}

int push(stack *st, void *data)
{
    if (((char *)st->head - (char *)st->stack) + st->type_size >= st->size)
        return 0;

    st->head = (unsigned char *)st->head + st->type_size;
    if (st->type_size == 4)
        *(unsigned int *)st->head = *(unsigned int *)data;
    else
        *(unsigned char *)st->head = *(unsigned char *)data;

    return 1;
}

void *pop(stack *st)
{
    if (((char *)st->head - (char *)st->stack) < 0)
        return NULL;

    void *head = st->head;
    st->head = (unsigned char *)st->head - st->type_size;

    return head;
}
/*1+2*3+((4+5)*(6+7*8)+(9+10)*16/4)*/
int main()
{
    stack operators, results;
    void *buf = mmap(1);
    int results_size = 1024 / sizeof(int);
    int operators_size = 1024;
    int string_buf_size = 1024 * 2;

    int *results_buf = (int *)buf;

    char *operators_buf = (char *)(results_buf + results_size);

    char *string_buf = (char *)(operators_buf + operators_size);

    init_simple_stack(&operators, operators_buf, operators_size, sizeof(char));
    init_simple_stack(&results, results_buf, results_size, sizeof(int));

    /*Проверка на валидность стоки ложится на
    get_math_expr_and_convert_RPN*/

    /*преобразование в польскую нотацию*/
    int j = 0, ch;
    while ((ch = getchar()) != EOF && ch != '\n' && j < string_buf_size)
    {
        /**/
        if (ch != ' ' && (ch < '(' || ch > '+') && 
            (ch < '0' || ch > '9') && ch != '-' && ch != '/')
        {
            print("An unknown symbol was encountered: ");
            putchar(ch);
            putchar('\n');
            munmap(buf, 1);
            return 1;
        }

        switch (ch)
        {
        case ' ':
            continue;
            break;
        case '*':
        case '/':
        {
            char *prev_oper = (char *)get_element(&operators);
            if (prev_oper && (*prev_oper == '*' || *prev_oper == '/'))
            {
                /*while((prev_oper = (char *)pop(&operators)) &&
                      (*prev_oper == '*' || *prev_oper == '/'))
                {
                    if(*prev_oper != '(')
                    {
                        string_buf[j] = ' ';
                        ++j;
                        string_buf[j] = *prev_oper;
                        ++j;
                    }
                }*/
                pop(&operators);
                string_buf[j] = ' ';
                ++j;
                string_buf[j] = *prev_oper;
                ++j;
                *prev_oper = 0;
            }
            push(&operators, (void *)&(ch));
            string_buf[j] = ' ';
            ++j;
        }
        break;
        case '+':
        case '-':
        {
            char *prev_oper = (char *)get_element(&operators);
            if (prev_oper && (*prev_oper == '*' || *prev_oper == '/'))
            {
                while ((prev_oper = (char *)get_element(&operators)) && *prev_oper != '(')
                {
                    prev_oper = (char *)pop(&operators);
                    string_buf[j] = ' ';
                    ++j;
                    string_buf[j] = *prev_oper;
                    ++j;
                    ;

                    *prev_oper = 0;
                }
            }
            /*if(oper && *oper != '(')
            {
                oper = (char *)pop(&operators);
                string_buf[j] = ' ';
                ++j;
                string_buf[j] = *oper;
                ++j;;
            }*/
            push(&operators, (void *)&(ch));
            string_buf[j] = ' ';
            ++j;
        }
        break;
        case '(':
            push(&operators, (void *)&(ch));
            break;
        case ')':
        {
            char *oper;
            while ((oper = (char *)pop(&operators)) && *oper != '(')
            {
                string_buf[j] = ' ';
                ++j;
                string_buf[j] = *oper;
                ++j;
                *oper = 0;
            }
            if (oper && *oper == '(')
            {
                *oper = 0;
            }
        }
        break;
        default:
            string_buf[j] = ch;
            ++j;
            break;
        }
    }

    char *oper;
    while ((oper = (char *)pop(&operators)) && j < string_buf_size)
    {
        string_buf[j] = *oper;
        ++j;
    }
    if(j >= string_buf_size)
    {
        munmap(buf, 1);
        return 1;
    }
    string_buf[j] = 0;

    int a = 0, b = 0;
    for (int i = 0; string_buf[i]; ++i)
    {

        if (string_buf[i] == '+' || string_buf[i] == '-' ||
            string_buf[i] == '*' || string_buf[i] == '/')
        {
            b = *(int *)pop(&results);
            a = *(int *)pop(&results);
            switch (string_buf[i])
            {
            case '*':
                a *= b;
                break;
            case '/':
                if (b == 0)
                {
                    munmap(buf, 1);
                    return 1;
                }
                a /= b;
                break;
            case '+':
                a += b;
                break;
            case '-':
                a -= b;
                break;
            }
            push(&results, &a);
            b = 0;
            a = 0;
            continue;
        }
        else
        {
            if (string_buf[i] == ' ')
                continue;
            while (string_buf[i] - '0' >= 0)
            {
                a = a * 10 + string_buf[i] - '0';
                ++i;
            }
            push(&results, &a);
            a = 0;
        }
    }

    print_int(*(int *)pop(&results));
    print("\n");

    munmap(buf, 1);
    return 0;
}
