#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
//打印系统相关信息
void pinrt_system_information()
{
	//输出主机信息
	system("echo 主机系统环境如下：");
	system("free -h");
	system("echo -n 主机系统版本：");
	system("uname -a");
	system("echo");

	system("sudo rm result.txt");//删除上一次的结果缓存
	system("sudo rm count.txt");//删除上一次的调用缓存
	system("sudo rm call_times.txt");//删除上一次的调用缓存
	system("sudo echo 主机系统环境如下： >> result.txt");
	system("sudo free -h >> result.txt");
	system("sudo echo -n 主机系统版本： >> result.txt");
	system("sudo uname -a >> result.txt");
	system("sudo echo >> result.txt");

	system("sudo echo 函数调用结果如下： >> count.txt");
}
//初始化沙盒系统
void init_sandbox()
{
	//编译hook.c文件为.so动态链接库
	system("pwd");
	system("ls");
	system("echo");
	system("gcc -fPIC -shared -o hook.so hook.c -ldl");
	system("gcc -o elf elf.c");
	system("gcc -o myecho myecho.c");
	//传入hook.so和可疑程序elf
	system("sudo docker cp elf e7311537000f:/home/");
	system("sudo docker cp result.txt e7311537000f:/home/");
	system("sudo docker cp count.txt e7311537000f:/home/");
	system("sudo docker cp myfile e7311537000f:/home/");
	system("sudo docker cp myecho e7311537000f:/home/");
	system("sudo docker cp hook.so e7311537000f:/home/");
}
//运行沙盒系统
void exec_sandbox()
{
	//输出系统信息
	system("sudo docker exec -it e73 /bin/bash -c 'echo docker容器环境如下：' ");
	system("sudo docker exec -it e73 /bin/bash -c 'free -h' ");
	system("sudo docker exec -it e73 /bin/bash -c 'echo -n docker容器版本：' ");
	system("sudo docker exec -it e73 /bin/bash -c 'uname -a' ");
	system("sudo docker exec -it e73 /bin/bash -c 'echo' ");
	system("sudo docker exec -it e73 /bin/bash -c 'echo -n user:' ");
	system("sudo docker exec -it e73 /bin/bash -c 'whoami' ");
	system("sudo docker exec -it e73 /bin/bash -c 'echo The detected program is elf' ");
	system("sudo docker exec -it e73 /bin/bash -c 'echo' ");

	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && echo docker容器环境如下： >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && free -h >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && echo -n docker容器版本： >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && uname -a >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && echo >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && echo -n user: >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && whoami >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && echo The detected program is elf >> result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && echo >> result.txt' ");
	//运行检测程序
	system("sudo docker exec -it e73 /bin/bash -c 'ls && echo && cd /home && LD_PRELOAD=./hook.so ./elf myecho' ");
	//将结果result.txt回传至主机中
	system("sudo docker cp  e7311537000f:/home/result.txt /home/");
	system("sudo docker cp  e7311537000f:/home/count.txt /home/");
}
//重置沙盒系统
void reload_sandbox()
{
	system("sudo docker exec -it e73 /bin/bash -c 'unset LD_PRELOAD' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && rm elf' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && rm result.txt' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && rm myfile' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && rm myecho' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && rm hook.so' ");
	system("sudo docker exec -it e73 /bin/bash -c 'cd /home && rm count.txt' ");
}
//输出程序用时
void print_use_time(long start_sec_time, long end_sec_time, long start_usec_time, long end_usec_time)
{
	double timeuse;
	timeuse = 1000000 * (end_sec_time - start_sec_time) + end_usec_time - start_usec_time;
	timeuse /= 1000000;
	printf("Time_use=%lfs\n", timeuse);
	FILE *fp = fopen("/home/time_use.txt", "a+"); //打开并在指定地点创建只写文件。
	fprintf(fp, "Time_use=%lfs\n", timeuse);
	fclose(fp);
}
int substring(char *str,char *str1)
{
	int x=0;
	char *p;//任意附个初始值
	do{
	    p=strstr(str,str1);//1.p指针指向strstr的返回值。3.再一次循环到 这里函数的参数发生变化，p重新指向strstr返回值，如此循环。
	    if(p != NULL) {
	       str=p+1;//2.str同样指向strstr返回值p的下一个地址。
	       x=x+1;
}
}
while(p!=NULL);
	return x;
}
void calculate_call_times()
{
	FILE *fp;
	fp = fopen("count.txt","rt");
	//求得文件的大小 
	fseek(fp,0,SEEK_END);
	int size = ftell(fp);
	printf("size:%d\n",size);
	rewind(fp);
	//申请一块能装下整个文件的空间 
	char *str = (char *)malloc(sizeof(char)*size);
	//读文件，每次读一个，共读size次 
	fread(str,1,size,fp);
	printf("%s\n",str);
	
	printf("函数调用次数统计：\n");
	char func0[10]="execve";
	char func1[10]="execv";
	char func2[10]="execl";
	char func3[10]="execle";
	char func4[10]="execlp";
	char func5[10]="execvp";
	char func6[10]="fexecve";
	char func7[10]="connect";
	char func8[20]="init_module";
	char func9[10]="write";
	char func10[10]="read";
	char func11[10]="open";
	char func12[10]="close";
	char func13[10]="send";
	int call_num[20];

	call_num[0]=substring(str,func0);
	call_num[1]=substring(str,func1);
	call_num[2]=substring(str,func2);
	call_num[3]=substring(str,func3);
	call_num[4]=substring(str,func4);
	call_num[5]=substring(str,func5);
	call_num[6]=substring(str,func6);
	call_num[7]=substring(str,func7);
	call_num[8]=substring(str,func8);
	call_num[9]=substring(str,func9);
	call_num[10]=substring(str,func10);
	call_num[11]=substring(str,func11);
	call_num[12]=substring(str,func12);
	call_num[13]=substring(str,func13);

	printf("the %s call times are %d\n",func0,call_num[0]);
	printf("the %s call times are %d\n",func1,call_num[1]);
	printf("the %s call times are %d\n",func2,call_num[2]);
	printf("the %s call times are %d\n",func3,call_num[3]);
	printf("the %s call times are %d\n",func4,call_num[4]);
	printf("the %s call times are %d\n",func5,call_num[5]);
	printf("the %s call times are %d\n",func6,call_num[6]);
	printf("the %s call times are %d\n",func7,call_num[7]);
	printf("the %s call times are %d\n",func8,call_num[8]);
	printf("the %s call times are %d\n",func9,call_num[9]);
	printf("the %s call times are %d\n",func10,call_num[10]);
	printf("the %s call times are %d\n",func11,call_num[11]);
	printf("the %s call times are %d\n",func12,call_num[12]);
	printf("the %s call times are %d\n",func13,call_num[13]);
	fclose(fp);

	//FILE *fp;
	system("sudo cat count.txt >> call_times.txt");
	fp = fopen("call_times.txt","a+");
	fprintf(fp,"\n");
	fprintf(fp,"函数调用次数如下：\n");
	fprintf(fp,"the %s call times are %d\n",func0,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func1,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func2,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func3,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func4,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func5,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func6,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func7,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func8,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func9,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func10,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func11,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func12,call_num[0]);
	fprintf(fp,"the %s call times are %d\n",func13,call_num[0]);

	fclose(fp);
	free(str);
}
int main()
{
	struct timeval start, end;
	//开始计时
	gettimeofday(&start, NULL);

	pinrt_system_information();
	init_sandbox();
	exec_sandbox();
	reload_sandbox();

	//结束及时
	gettimeofday(&end, NULL);
	print_use_time(start.tv_sec, end.tv_sec, start.tv_usec, end.tv_usec);

	//统计调用次数
	calculate_call_times();
	return 0;
}
