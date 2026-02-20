#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 60

typedef struct record
{
    char buf[BUFSIZE];
    int count;
}record;

typedef enum command_codes
{
    add,
    query,
    list,
    code_error
} command_codes;

record read_buf[BUFSIZE];

typedef int (*function_commands)(const char *file_name, const char *id);

int str_cmp_case_sensitive(const char *str1, const char *str2)
{
    for (int i = 0; str1[i] || str2[i]; ++i)
    {
        if ((str1[i]) != str2[i])
        {
            return 0;
        }
    }
    return 1;
}

int str_cmp(const char *str1, const char *str2)
{
    for (int i = 0; str1[i] || str2[i]; ++i)
    {
        if ((str1[i] | 0x20) != (str2[i] | 0x20))
        {
            return 0;
        }
    }
    return 1;
}

record *find_record(int fd, const char *id, int *cur_pos)
{
    *cur_pos = 0;

    int readed;
    while((readed = read(fd, read_buf, sizeof(read_buf))))
    {
        readed /= sizeof(record);
        for(int i = 0; i < readed; ++i)
        {
            *cur_pos += sizeof(record);
            if(str_cmp(id, read_buf[i].buf))
                return &read_buf[i];
        }
    }

    return NULL;
}

int command_add(const char *file_name, const char *id)
{
    int file = open(file_name, O_RDWR);
    if(file < 0)
    {
        printf("%s error: can't open file %s\n", __FUNCTION__, file_name);
        return 0;
    }
    int record_pos;
    record *precord = find_record(file, id, &record_pos);
    if (precord)
    {
        precord->count += 1;
        lseek(file, record_pos - sizeof(record), SEEK_SET);
        write(file, precord, sizeof(record));
    }
    else
    {
        record tmp;
        int i = 0;
        for (; id[i]; ++i)
        {
            tmp.buf[i] = id[i];
        }
        tmp.buf[i] = 0;
        tmp.count = 1;
        lseek(file, 0, SEEK_END);
        write(file, &tmp, sizeof(record));
    }
    return 1;
}

int command_query(const char *file_name, const char *id)
{
    int file = open(file_name, O_RDWR);
    if (file < 0)
    {
        printf("%s error: can't open file %s\n", __FUNCTION__, file_name);
        return 0;
    }
    
    int record_pos;
    record *precord = find_record(file, id, &record_pos);
    if(precord)
    {
        printf("%d\n", precord->count);
        return 1;
    }
    printf("%s not found\n", id);
    return 0;
}

int command_list(const char *file_name, const char *id)
{
    int file = open(file_name, O_RDWR);
    if (file < 0)
    {
        printf("%s error: can't open file %s\n", __FUNCTION__, file_name);
        return 0;
    }
    int readed;
    while ((readed = read(file, read_buf, sizeof(read_buf))))
    {
        readed /= sizeof(record);
        for (int i = 0; i < readed; ++i)
        {
            printf("%s: %d\n", read_buf[i].buf, read_buf[i].count);
        }
    }

    return 1;
}



const function_commands functions[] = {&command_add, &command_query, &command_list};

int process_command(const char *command)
{
    if(str_cmp("add", command))
        return add;
    if(str_cmp("list", command))
        return list;
    if(str_cmp("query", command))
        return query;

    return code_error;
}

int manage_commands(const char *file_name, const char *command, const char *id)
{
    int fid;
    if ((fid = process_command(command)) == code_error)
    {
        printf("Unknow command '%s'\n", command);
        return 0;
    }
    return functions[fid](file_name, id);
}

int main(int argc, const char *argv[])
{

    if(argc == 4)
    {
        manage_commands(argv[1], argv[2], argv[3]);
    }
    else if (argc == 3)
    {
        manage_commands(argv[1], argv[2], 0);
    }
    else
    {
        printf("Args count error\n");
        return 1;
    }
    



    return 0;
}