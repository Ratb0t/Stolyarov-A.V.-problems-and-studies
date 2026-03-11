#include "context.h"

context *create_and_init_context()
{
    context *shell = malloc(sizeof(context));
    if (shell == NULL)
    {
        printf("Context create error: Failed to allocate interpreter\n");
        return NULL;
    }
    shell->alzr = create_analyzator();
    if (shell->alzr == NULL)
    {
        free(shell);
        return NULL;
    }

    shell->proc_hanler = create_process_handle();
    if (shell->proc_hanler == NULL)
    {
        destroy_analyzator(shell->alzr);
        free(shell);
        return NULL;
    }
        
    init_analizator(shell->alzr);
    init_process_handle(shell->proc_hanler);
    shell->code.major_code = ok;
    shell->code.minore_code.raw = 0;
    return shell;
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