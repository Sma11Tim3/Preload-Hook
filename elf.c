#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
void write_read_test()
{
	int fd = open("myfile", O_WRONLY); //open
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	const char*msg = "hello open\n";
	char *mss = "Hello world\n";
	int count = 3;
	while (count--)
	{
		write(fd, msg, strlen(msg)); //write
		read(fd, mss, strlen(mss)); //read
	}
	close(fd);//close
}
void execve_test(int argc, char *argv[])
{
	char *newargv[] = { NULL, "hello", "world", NULL };
	char *newenviron[] = { NULL };

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	newargv[0] = argv[1];

	execve(argv[1], newargv, newenviron);
	perror("execve");   /* execve() only returns on error */
	exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
	write_read_test();
	execve_test(argc, argv);
	return 0;
}
