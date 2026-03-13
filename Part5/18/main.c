#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define clear_stdout fflush(stdout)
#define dup_desc STDOUT_FILENO

#ifdef VARIANT_a
#define flags_open O_CREAT | O_TRUNC | O_WRONLY
#endif
#ifdef VARIANT_b
#undef dup_desc
#define dup_desc STDIN_FILENO
#define flags_open O_RDONLY
#undef clear_stdout
#define clear_stdout
#endif
#ifdef VARIANT_c
#define flags_open O_CREAT | O_APPEND | O_WRONLY
#endif
#ifdef VARIANT_d
#define flags_open O_APPEND | O_WRONLY
#endif
char **format_cmd_line(char *argv[])
{
	return &argv[2];
}

int set_new_stdout(int new_fd, int *save)
{
	clear_stdout;
	*save = dup(dup_desc);

	dup2(new_fd, dup_desc);
	close(new_fd);
	
	return 1;
}

int create_process(char **cmd_line)
{
	int pid = fork();
	if(pid == 0)
	{
		execvp(*cmd_line, cmd_line);
		perror("Command error");
		close(STDOUT_FILENO);
		exit(2);
	}
	return pid;
}

int open_file(char *file_name)
{
	int fd = open(file_name, flags_open, 0666);
	if (fd == -1)
	{
		perror(file_name);
		exit(1);
	}
	return fd;
}


int main(int argc, char *argv[])
{
	if(argc < 3)
		printf("Not enougth args\n");

	int std_out;

	set_new_stdout(open_file(argv[1]), &std_out);

	create_process(format_cmd_line(argv));

	wait(NULL);

	set_new_stdout(std_out, &std_out);

	close(std_out);

	return 0;
}
