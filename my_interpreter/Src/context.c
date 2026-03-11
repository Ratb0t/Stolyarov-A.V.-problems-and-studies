#include "context.h"

context *create_and_init_context()
{
    context *cnt = malloc(sizeof(context));
    if (cnt == NULL)
    {
        printf("Context create error: Failed to allocate interpreter\n");
        return NULL;
    }
    cnt->alzr = create_analyzator();
    if (cnt->alzr == NULL)
    {
        free(cnt);
        return NULL;
    }

    cnt->proc_hanler = create_process_handle();
    if (cnt->proc_hanler == NULL)
    {
        destroy_analyzator(cnt->alzr);
        free(cnt);
        return NULL;
    }
        
    init_analizator(cnt->alzr);
    init_process_handle(cnt->proc_hanler);
    cnt->code.major_code = ok;
    cnt->code.minore_code.raw = 0;
    return cnt;
}
void destroy_context(context *cnt)
{
    destroy_analyzator(cnt->alzr);
    destroy_process_handle(cnt->proc_hanler);
    free(cnt);
    return;
}

void context_reset_analizator(context *cnt)
{
    reset_analizator(cnt->alzr);
    cnt->code = cnt->alzr->code;
    return;
}