
extern int print(const char *);
extern int open(const char *pathname, int flags, int mode);
extern int close(int fd);
extern int write(int fd, void* buf, int size);
extern int read(int fd, void *buf, int size);

#define O_RDONLY        0x0
#define O_WRONLY        0x1
#define O_RDWR          0x2
#define O_CREAT         0x40
#define O_TRUNC         0x200
    ;
unsigned char *buf[4096];
int main(int argc, char **argv)
{

    if(argc != 3)
    {
        print("Wrong args count\n");
        return 1;
    }

    int src = open(argv[1], O_RDONLY, 0x1FF);
    int dest = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0x1B6);
    int readed = 0;
    while ((readed = read(src, buf, 4095)))
    {
        readed = write(dest, buf, readed);
    }

    close(src);
    close(dest);
    return 0;
}