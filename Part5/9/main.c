#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

const char *get_type(struct stat *buf)
{
    if (S_ISLNK(buf->st_mode))
        return"Link";
    if (S_ISREG(buf->st_mode))
        return "File";
    if (S_ISDIR(buf->st_mode))
        return "Directory";
    if (S_ISCHR(buf->st_mode))
        return "Character device";
    if (S_ISBLK(buf->st_mode))
        return "Block device";
    if (S_ISFIFO(buf->st_mode))
        return "FIFO chanle (pipe)";
    if (S_ISSOCK(buf->st_mode))
       return "Socket";
    
    return NULL;
}

void print_info(struct stat *buf)
{
    printf("Device id %38s %ld\n"," ", buf->st_dev);
    printf("I-node num %37s %ld\n"," ", buf->st_ino);

    printf("Right %42s %#o\n"," ", buf->st_mode & 07777);
    printf("Type %43s %s\n", " ", get_type(buf));

    printf("Link count %37s %ld\n", " ", buf->st_nlink);
    printf("Owner id %39s %d\n", " ",buf->st_uid);
    printf("User group id %34s %d\n", " ",buf->st_gid);
    printf("Id device (specific for system) %16s %ld\n", " ", buf->st_rdev);
    printf("Size %43s %ld byte\n", " ", buf->st_size);
    printf("IO block size %34s %ld byte\n", " ", buf->st_blksize);
    printf("Count of 512-byte blocks occupied by the file %2s %ld\n", 
        " ", buf->st_blocks); /*number of blocks allocated for this object*/
    printf(
        "Date and time of last file access                %s"
        "Date and time of last content modification       %s"
        "Date and time of last property change            %s", 
        ctime(&buf->st_atime), ctime(&buf->st_mtime), ctime(&buf->st_ctime));
}

int file_info(const char *filename)
{

    struct stat buf;
    if(!lstat(filename, &buf))
        print_info(&buf);
    if(S_ISLNK(buf.st_mode))
    {
        putchar('\n');
        putchar('\n');
        if(!stat(filename, &buf))
            print_info(&buf);
        else
            printf("dangling\n");
    }
    
    return 1;
}


int main(int argc, const char *argv[])
{

    for(int i = 1; i < argc; ++i)
        file_info(argv[i]);
    return 0;
}