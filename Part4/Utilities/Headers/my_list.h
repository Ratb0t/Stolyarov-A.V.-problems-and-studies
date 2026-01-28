#ifndef REINCLUDE_PROTECTION_MY_LIST
#define REINCLUDE_PROTECTION_MY_LIST
struct node
{
    void *data_holder;
    struct node *next;
};

typedef struct node *my_list_iterator;
typedef const struct node **my_list_head, **my_list_tail;

struct my_list;
typedef struct my_list my_list;


my_list *my_list_create();
/*******************************************************************************/
int my_list_destroy(my_list *lst);
/*******************************************************************************/
int my_list_revers(my_list *lst);
/*******************************************************************************/
int my_list_push_front(my_list *list, void *data);

int my_list_push_back(my_list *list, void *data);
/*******************************************************************************/
void *my_list_delete_item(my_list *lst, unsigned int pos);
/*******************************************************************************/
unsigned int my_list_get_len(const my_list *lst);

my_list_iterator my_list_get_item(const my_list *lst, unsigned int pos);

my_list_iterator my_list_get_first(const my_list *lst);

my_list_iterator my_list_get_last(const my_list *lst);

my_list_head my_list_get_head(const my_list *lst);

my_list_tail my_list_get_tail(const my_list *lst);
/*******************************************************************************/

#endif //! REINCLUDE_PROTECTION_MY_LIST