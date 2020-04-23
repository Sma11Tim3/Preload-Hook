/*
编译hook文件
gcc -fPIC -shared -o hook.so hook.c -ldl
-shared选项表示创建一个动态链接库。输入文件可以是源文件、汇编文件或者目标文件。
-fPIC 选项作用于编译阶段，告诉编译器产生与位置无关代码
-ldl表示程序中使用dlopen、dlsym、dlclose、dlerror 显示加载动态库。 
*/
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#if defined(RTLD_NEXT)
#define REAL_LIBC RTLD_NEXT
#else
#define REAL_LIBC ((void *)-1L)
#endif

#define FN(ptr, type, name, args) ptr = (type(*) args)dlsym(REAL_LIBC, name)
#define MAX_SIZE 1024

char current_absolute_path[MAX_SIZE];

int Ispath(const char *filename)
{
	for (int i = 0; i < strlen(filename); i++)
	{
		if (filename[i] == '/')
		{
			return 1;
		}
	}
	return 0;
}

// //获取当前绝对路径
void get_current_absolute_path()
{
	getcwd(current_absolute_path, sizeof(current_absolute_path));
}

int open_count = 0;
int close_count = 0;
int write_count = 0;
int read_count = 0;
int fwrite_count = 0;

//int main(int argc,char　* argv[],char * envp[])
//第三个参数用来在程序运行时获取系统的环境变量，操作系统运行程序时通过envp 参数将系统环境变量传递给程序
//Hook function execve()
typedef int (*EXECVE)(const char *filename, char *const argv[], char *const envp[]);
int execve(const char *filename, char *const argv[], char *const envp[])
{
	static void *handle = NULL;
	static EXECVE old_execve = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_execve = (EXECVE)dlsym(handle, "execve");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "execve ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---execve-- successful!\n");
	FILE *fp;
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---execve--- successful!\n");
	fclose(fp); //关闭指定文件
	if (Ispath(filename))
	{
		printf("filepath: %s\n", filename);
		printf("filename: %s\n", argv[0]);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", filename);
		fprintf(fp, "filename: %s\n", argv[0]);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		printf("filepath: %s/%s\n", current_absolute_path, filename);
		printf("filename: %s\n", argv[0]);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, filename);
		fprintf(fp, "filename: %s\n", argv[0]);
		fclose(fp); //关闭指定文件
	}
	// for(int i=0;envp[i]!=NULL;i++)
	// 	printf("envp%d:%s\n",i,envp[i]);

	printf("The function address:%p\n", old_execve);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The function address:%p\n", old_execve);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_execve(filename, (char **)argv, (char **)envp);
}

//Hook function execv()
typedef int (*EXECV)(const char *filename, char *const argv[]);
int execv(const char *filename, char *const argv[])
{
	static void *handle = NULL;
	static EXECV old_execv = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_execv = (EXECV)dlsym(handle, "execv");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "execv ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---execv--- successful!\n");
	FILE *fp;
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---execv--- successful!\n");
	fclose(fp); //关闭指定文件
	if (Ispath(filename))
	{
		printf("filepath: %s\n", filename);
		printf("filename: %s\n", argv[0]);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", filename);
		fprintf(fp, "filename: %s\n", argv[0]);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		printf("filepath: %s/%s\n", current_absolute_path, filename);
		printf("filename: %s\n", argv[0]);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, filename);
		fprintf(fp, "filename: %s\n", argv[0]);
		fclose(fp); //关闭指定文件
	}
	printf("The function address:%p\n", old_execv);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The function address:%p\n", old_execv);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_execv(filename, (char **)argv);
}
//Hook function execl()
typedef int (*EXECL)(const char *path, const char *arg, ...);
int execl(const char *path, const char *arg, ...)
{
	static void *handle = NULL;
	static EXECL old_execl = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_execl = (EXECL)dlsym(handle, "execl");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "execl ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---execl--- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---execl--- successful!\n");
	fclose(fp); //关闭指定文件
	if (Ispath(path))
	{
		printf("filepath: %s\n", path);
		printf("filename: %s\n", arg);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", path);
		fprintf(fp, "filename: %s\n", arg);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		printf("filepath: %s/%s\n", current_absolute_path, path);
		printf("filename: %s\n", arg);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, path);
		fprintf(fp, "filename: %s\n", arg);
		fclose(fp); //关闭指定文件
	}
	printf("The function address:%p\n", old_execl);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The function address:%p\n", old_execl);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_execl(path, arg);
}

//Hook the function execle
typedef int (*EXECLE)(const char *pathname, const char *arg0, ...);
int execle(const char *pathname, const char *arg0, ...)
{
	static void *handle = NULL;
	static EXECLE old_execle = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_execle = (EXECLE)dlsym(handle, "execle");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "execle ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---execle--- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---execle--- successful!\n");
	fclose(fp); //关闭指定文件
	if (Ispath(pathname))
	{
		printf("filepath: %s\n", pathname);
		printf("filename: %s\n", arg0);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", pathname);
		fprintf(fp, "filename: %s\n", arg0);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		printf("filepath: %s/%s\n", current_absolute_path, pathname);
		printf("filename: %s\n", arg0);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, pathname);
		fprintf(fp, "filename: %s\n", arg0);
		fclose(fp); //关闭指定文件
	}
	printf("The function address:%p\n", old_execle);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The function address:%p\n", old_execle);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_execle(pathname, arg0);
}

//Hook the function execlp
typedef int (*EXECLP)(const char *filename, const char *arg0, ...);
int execlp(const char *filename, const char *arg0, ...)
{
	static void *handle = NULL;
	static EXECLP old_execlp = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_execlp = (EXECLP)dlsym(handle, "execlp");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "execlp ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---execlp--- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---execlp--- successful!\n");
	fclose(fp); //关闭指定文件
	if (Ispath(filename))
	{
		printf("filepath: %s\n", filename);
		printf("filename: %s\n", arg0);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", filename);
		fprintf(fp, "filename: %s\n", arg0);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		printf("filepath: %s/%s\n", current_absolute_path, filename);
		printf("filename: %s\n", arg0);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, filename);
		fprintf(fp, "filename: %s\n", arg0);
		fclose(fp); //关闭指定文件
	}
	printf("The function address:%p\n", old_execlp);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The function address:%p\n", old_execlp);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_execlp(filename, arg0);
}

//Hook the function execvp
typedef int (*EXECVP)(const char *filename, char *const argv[]);
int execvp(const char *filename, char *const argv[])
{
	static void *handle = NULL;
	static EXECVP old_execvp = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_execvp = (EXECVP)dlsym(handle, "execvp");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "execvp ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---execvp--- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---execvp--- successful!\n");
	fclose(fp); //关闭指定文件
	if (Ispath(filename))
	{
		printf("filepath: %s\n", filename);
		printf("filename: %s\n", argv[0]);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", filename);
		fprintf(fp, "filename: %s\n", argv[0]);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		printf("filepath: %s/%s\n", current_absolute_path, filename);
		printf("filename: %s\n", argv[0]);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, filename);
		fprintf(fp, "filename: %s\n", argv[0]);
		fclose(fp); //关闭指定文件
	}
	printf("The function address:%p\n", old_execvp);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The function address:%p\n", old_execvp);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_execvp(filename, (char **)argv);
}

//Hook the function
typedef int (*FEXECVE)(int fd, char *const argv[], char *const envp[]);
int fexecve(int fd, char *const argv[], char *const envp[])
{
	static void *handle = NULL;
	static FEXECVE old_fexecve = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_fexecve = (FEXECVE)dlsym(handle, "fexecve");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "fexecve ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---fexecve-- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---fexecve--- successful!\n");
	fclose(fp); //关闭指定文件
	printf("filename: %s\n", argv[0]);
	// for(int i=0;envp[i]!=NULL;i++)
	// 	printf("envp%d:%s\n",i,envp[i]);
	printf("The function address:%p\n", old_fexecve);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "filename: %s\n", argv[0]);
	fprintf(fp, "The function address:%p\n", old_fexecve);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_fexecve(fd, (char **)argv, (char **)envp);
}

//Hook function connect()
typedef int (*CONNECT)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	static void *handle = NULL;
	static CONNECT old_connect = NULL;
	//int ret_code = old_connect(sockfd,addr,addrlen);
	int tmp_errno = errno;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_connect = (CONNECT)dlsym(handle, "connect");
	}
	system("sudo tcpdump -i any -n port 7890 or host 192.168.43.2 -c 50 -w /home/sma11-tim3/Linux_tcpdump/test.pcap");
	// if(ret_code == -1 && tmp_errno != EINPROGRESS)
	// {
	//     return ret_code;
	// }
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "connect ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---connect--- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---connect--- successful!\n");
	fclose(fp); //

	printf("The function address:%p\n", old_connect);
	printf("The Traffic packet has saved as pcap file.");
	printf("The pcap file path: /home/sma11-tim3/Linux_tcpdump/test.pcap");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The function address:%p\n", old_connect);
	fprintf(fp, "The Traffic packet has saved as pcap file.");
	fprintf(fp, "The pcap file path: /home/sma11-tim3/Linux_tcpdump/test.pcap");
	fclose(fp); //关闭指定文件
	//printf("%d\n",old_connect(sockfd,addr,addrlen));
	return old_connect(sockfd, addr, addrlen);
}

// init_module是默认的模块的入口，如果你想指定其他的函数作为模块的入口就需要module_init函数来指定。

// init_module()是真正的入口，module_init是宏，如果在模块中使用，最终还是要转换到init_module()上。
//如果不是在模块中使用，module_init可以说没有什么作用。使用module_init方便代码在模块和非模块间移植。

//Hook function init_module()
typedef int (*INIT_MODULE)(void *module_image, unsigned long len, const char *param_values);
int init_module(void *module_image, unsigned long len, const char *param_values)
{
	static void *handle = NULL;
	static INIT_MODULE old_init_module = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_init_module = (INIT_MODULE)dlsym(handle, "init_module");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "init_module ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---lkm load--- successful!\n");
	printf("The function address:%p\n", old_init_module);
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---lkm load--- successful!\n");
	fprintf(fp, "The function address:%p\n", old_init_module);
	fclose(fp); //关闭指定文件
	return old_init_module(module_image, len, param_values);
}

//Hook function wirte()
typedef ssize_t (*WRITE)(int fd, const void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count)
{
	static void *handle = NULL;
	static WRITE old_write = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_write = (WRITE)dlsym(handle, "write");
	}
	//Calculate the write numbers
	write_count++;
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "write ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---write--- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---write--- successful!\n");
	fclose(fp); //关闭指定文件
	int count_num = 0;
	while (count > MAX_SIZE)
	{
		count = count / MAX_SIZE;
		count_num++;
	}
	if (count_num == 0)
	{
		printf("The write sizes are <%lu> byte\n", count);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The write sizes are <%lu> byte\n", count);
		fclose(fp); //关闭指定文件
	}
	if (count_num == 1)
	{
		printf("The write sizes are <%lu> KB\n", count);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The write sizes are <%lu> KB\n", count);
		fclose(fp); //关闭指定文件
	}
	if (count_num == 2)
	{
		printf("The write sizes are <%lu> MB\n", count);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The write sizes are <%lu> MB\n", count);
		fclose(fp); //关闭指定文件
	}
	if (count_num == 3)
	{
		printf("The write sizes are <%lu> GB\n", count);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The write sizes are <%lu> GB\n", count);
		fclose(fp); //关闭指定文件
	}
	if (count_num == 4)
	{
		printf("The write sizes are <%lu> TB\n", count);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The write sizes are <%lu> TB\n", count);
		fclose(fp); //关闭指定文件
	}
	printf("buf_address:%p\n", buf);
	printf("The function address:%p\n", old_write);
	printf("write_count:%d\n", write_count);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "buf_address:%p\n", buf);
	fprintf(fp, "The function address:%p\n", old_write);
	fprintf(fp, "write_count:%d\n", write_count);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件

	return old_write(fd, buf, count);
}

//Hook function read()
typedef ssize_t (*READ)(int fd, void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count)
{
	static void *handle = NULL;
	static READ old_read = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_read = (READ)dlsym(handle, "read");
	}
	//Calculate the read numbers
	read_count++;
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "read ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---read--- successful!\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---read--- successful!\n");
	fclose(fp); //关闭指定文件

	printf("The read sizes are <%lu> byte\n", count);
	printf("buf_address:%p\n", buf);
	printf("The function address:%p\n", old_read);
	printf("read_count:%d\n", read_count);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "The read sizes are <%lu> byte\n", count);
	fprintf(fp, "buf_address:%p\n", buf);
	fprintf(fp, "The function address:%p\n", old_read);
	fprintf(fp, "read_count:%d\n", read_count);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件

	return old_read(fd, buf, count);
}

//Hook function open1()
typedef int (*OPEN)(const char *pathname, int flags, mode_t mode);
int open(const char *pathname, int flags, mode_t mode)
{
	static void *handle = NULL;
	static OPEN old_open = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_open = (OPEN)dlsym(handle, "open");
	}
	//Calculate the open numbers
	open_count++;
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "open ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---open--- successful!\n");
	printf("The function address:%p\n", old_open);
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---open--- successful!\n");
	fprintf(fp, "The function address:%p\n", old_open);
	fclose(fp); //关闭指定文件
	//printf("The filepath is <%s>\n",pathname);
	if (Ispath(pathname))
	{
		const char *filename;
		filename = pathname;
		printf("filepath: %s\n", pathname);
		printf("filename: %s\n", filename);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", pathname);
		fprintf(fp, "filename: %s\n", filename);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		const char *filename;
		filename = pathname;
		printf("filepath: %s/%s\n", current_absolute_path, pathname);
		printf("filename: %s\n", filename);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, pathname);
		fprintf(fp, "filename: %s\n", filename);
		fclose(fp); //关闭指定文件
	}
	printf("flags is :%d\n", flags);
	if (flags == 0)
	{
		printf("The openflags is [O_RDONLY](only read)\n");
		printf("\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_RDONLY](only read)\n");
		fprintf(fp, "\n");
		fclose(fp); //关闭指定文件
		if (mode == 64)
		{
			printf("The openmode is [O_CREAT]");
			//若文件不存在，则创建它，需要使用mode选项。来指明新文件的访问权限
			printf("\n");
			fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
			fprintf(fp, "The openmode is [O_CREAT]");
			fprintf(fp, "\n");
			fclose(fp); //关闭指定文件
		}
		if (mode == 1024)
		{
			printf("The openmode is [O_APPEND]");
			//追加写，如果文件已经有内容，这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容
			printf("\n");
			fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
			fprintf(fp, "The openmode is [O_APPEND]");
			fprintf(fp, "\n");
			fclose(fp); //关闭指定文件
		}
	}
	if (flags == 1)
	{
		printf("The openflags is [O_WRONLY](only write)\n");
		printf("\n");
		FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_WRONLY](only write)\n");
		fprintf(fp, "\n");
		fclose(fp); //关闭指定文件
		if (mode == 64)
		{
			printf("The openmode is [O_CREAT]");
			//若文件不存在，则创建它，需要使用mode选项。来指明新文件的访问权限
			printf("\n");
			fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
			fprintf(fp, "The openmode is [O_CREAT]");
			fprintf(fp, "\n");
			fclose(fp); //关闭指定文件
		}
		if (mode == 1024)
		{
			printf("The openmode is [O_APPEND]");
			//追加写，如果文件已经有内容，这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容
			printf("\n");
			fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
			fprintf(fp, "The openmode is [O_APPEND]");
			fprintf(fp, "\n");
			fclose(fp); //关闭指定文件
		}
	}
	if (flags == 2)
	{
		printf("The openflags is [O_RDWR](read and write)\n");
		printf("\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_RDWR](read and write)\n");
		fprintf(fp, "\n");
		fclose(fp); //关闭指定文件
		if (mode == 64)
		{
			printf("The openmode is [O_CREAT]");
			//若文件不存在，则创建它，需要使用mode选项。来指明新文件的访问权限
			printf("\n");
			fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
			fprintf(fp, "The openmode is [O_CREAT]");
			fprintf(fp, "\n");
			fclose(fp); //关闭指定文件
		}
		if (mode == 1024)
		{
			printf("The openmode is [O_APPEND]");
			//追加写，如果文件已经有内容，这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容
			printf("\n");
			fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
			fprintf(fp, "The openmode is [O_APPEND]");
			fprintf(fp, "\n");
			fclose(fp); //关闭指定文件
		}
	}
	if (flags == 64)
	{
		printf("The openflags is [O_CREAT](creat the file)\n");
		printf("\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_CREAT](creat the file)\n");
		fprintf(fp, "\n");
		fclose(fp); //关闭指定文件
	}
	if (flags == 1024)
	{
		printf("The openflags is [O_APPEND](append the file)\n");
		printf("\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_APPEND](append the file)\n");
		fprintf(fp, "\n");
		fclose(fp); //关闭指定文件
	}
	printf("open_count:%d\n", open_count);
	printf("\n");
	fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "open_count:%d\n", open_count);
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return old_open(pathname, flags, mode);
}

//Hook function open2()
typedef int (*OPEN2)(const char *pathname, int flags);
int open2(const char *pathname, int flags)
{
	static void *handle = NULL;
	static OPEN2 old_open = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_open = (OPEN2)dlsym(handle, "open");
	}
	//Calculate the open numbers
	open_count++;
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "open ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---open--- successful!\n");
	printf("The function address:%p\n", old_open);
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---open--- successful!\n");
	fprintf(fp, "The function address:%p\n", old_open);
	fclose(fp); //关闭指定文件
	if (Ispath(pathname))
	{
		const char *filename;
		filename = pathname;
		printf("filepath: %s\n", pathname);
		printf("filename: %s\n", filename);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s\n", pathname);
		fprintf(fp, "filename: %s\n", filename);
		fclose(fp); //关闭指定文件
	}
	else
	{
		get_current_absolute_path();
		const char *filename;
		filename = pathname;
		printf("filepath: %s/%s\n", current_absolute_path, pathname);
		printf("filename: %s\n", filename);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "filepath: %s/%s\n", current_absolute_path, pathname);
		fprintf(fp, "filename: %s\n", filename);
		fclose(fp); //关闭指定文件
	}
	printf("%d\n", flags);
	if (flags == 0)
	{
		printf("The openflags is [O_RDONLY](only read)\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_RDONLY](only read)\n");
		fclose(fp); //关闭指定文件
	}
	if (flags == 1)
	{
		printf("The openflags is [O_WRONLY](only write)\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_WRONLY](only write)\n");
		fclose(fp); //关闭指定文件
	}
	if (flags == 2)
	{
		printf("The openflags is [O_RDWR](read and write)\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_RDWR](read and write)\n");
		fclose(fp); //关闭指定文件
	}
	if (flags == 3)
	{
		printf("The openflags is [O_APPEND](append the file)\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_APPEND](append the file)\n");
		fclose(fp); //关闭指定文件
	}
	if (flags == 4)
	{
		printf("The openflags is [O_CREAT](creat the file)\n");
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The openflags is [O_CREAT](creat the file)\n");
		fclose(fp); //关闭指定文件
	}
	return old_open(pathname, flags);
}

//Hook the function close()
typedef int (*CLOSE)(int fd);
int close(int fd)
{
	static void *handle = NULL;
	static CLOSE old_close = NULL;
	if (!handle)
	{
		handle = dlopen("Libc.so.6", RTLD_LAZY);
		old_close = (CLOSE)dlsym(handle, "close");
	}
	close_count++;
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "close ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---close--- successful!\n");
	printf("The function address:%p\n", old_close);
	printf("\n");
	printf("close_count:%d\n", close_count);
	printf("-------------------------------------------");
	printf("\n");
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Hook the function ---close--- successful!\n");
	fprintf(fp, "The function address:%p\n", old_close);
	fprintf(fp, "\n");
	fprintf(fp, "close_count:%d\n", close_count);
	fprintf(fp, "-------------------------------------------");
	fprintf(fp, "\n");
	fclose(fp); //关闭指定文件
	return 0;
}

typedef ssize_t (*SEND)(int sockfd, const void *buf, size_t len, int flags);
ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
	static void *handle = NULL;
	static SEND old_send = NULL;
	if (!handle)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_send = (SEND)dlsym(handle, "send");
	}
	//print log
	FILE *fpp = fopen("/home/count.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fpp, "send ");
	fclose(fpp); //关闭指定文件

	printf("Hook the function ---send--- successful!\n");
	printf("The function address:%p\n", old_send);
	printf("buf_address:%p\n", buf);
	FILE *fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
	int len_num = 0;
	while (len > MAX_SIZE)
	{
		len = len / MAX_SIZE;
		len_num++;
	}
	if (len_num == 0)
	{
		printf("The send sizes are <%lu> byte\n", len);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The send sizes are <%lu> byte\n", len);
		fclose(fp); //关闭指定文件
	}
	if (len_num == 1)
	{
		printf("The send sizes are <%lu> KB\n", len);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The send sizes are <%lu> KB\n", len);
		fclose(fp); //关闭指定文件
	}
	if (len_num == 2)
	{
		printf("The send sizes are <%lu> MB\n", len);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The send sizes are <%lu> MB\n", len);
		fclose(fp); //关闭指定文件
	}
	if (len_num == 3)
	{
		printf("The send sizes are <%lu> GB\n", len);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The send sizes are <%lu> GB\n", len);
		fclose(fp); //关闭指定文件
	}
	if (len_num == 4)
	{
		printf("The send sizes are <%lu> TB\n", len);
		fp = fopen("/home/result.txt", "a+"); //打开并在指定地点创建只写文件。
		fprintf(fp, "The send sizes are <%lu> TB\n", len);
		fclose(fp); //关闭指定文件
	}
	return old_send(sockfd, buf, len, flags);
}

//hook的目标是strcmp，所以typedef了一个STRCMP函数指针
//hook的目的是要控制函数行为，从原库libc.so.6中拿到strcmp指针，保存成old_strcmp以备调用

//Hook function strcmp()
// typedef int(*STRCMP)(const char*, const char*);
// int strcmp(const char *s1, const char *s2)
// {
//     static void *handle = NULL;
//     static STRCMP old_strcmp = NULL;

//     if( !handle )
//     {
//         handle = dlopen("libc.so.6", RTLD_LAZY);
//         old_strcmp = (STRCMP)dlsym(handle, "strcmp");
//     }
//     //printf("%s %s ",s1,s2);
//     //if((*s1)!="tcp"||(*s1)!="80"||(*s1)!="udp")
//     //if((*s1)=="tcp")
//     //printf("\n");
//     printf("Hook the function ---strcmp--- successful!. string1=<%s> string2=<%s>\n", s1, s2);
//     return old_strcmp(s1, s2);
// }
