#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char *program, char **args) {
	if(fork() == 0) {
		exec(program, args);
		exit(0);
	}
	return; 
}

int main(int argc, char *argv[]){
	char buf[2048];                 // 读入时使用的内存池
	char *p = buf, *last_p = buf;   // 当前参数的结束、开始指针
	char *argsbuf[128];             // 全部参数列表，字符串指针数组，包含 argv 传进来的参数和 stdin 读入的参数
	char **args = argsbuf;          // 指向 argsbuf 中第一个从 stdin 读入的参数
	for(int i=1;i<argc;i++) {		
		*args = argv[i];            // 将 argv 提供的参数加入到最终的参数列表中
		args++;
	}
	char **pa = args;               // 开始读入参数
	while(read(0, p, 1) != 0) {     // 读入一个参数完成（以空格分隔，如 `echo hello world`，则 hello 和 world 各为一个参数）
		if(*p == ' ' || *p == '\n') {
			*p = '\0';	            // 将空格替换为 \0 分割开各个参数，这样可以直接使用内存池中的字符串作为参数字符串而不用额外开辟空间
			*(pa++) = last_p;
			last_p = p+1;

			if(*p == '\n') {
				*pa = 0;            // 读入一行完成,参数列表末尾用 null 标识列表结束
				run(argv[1], argsbuf); // 执行最后一行指令
				pa = args;          // 重置读入参数指针，准备读入下一行
			}
		}
		p++;
	}
	if(pa != args) {                // 如果最后一行不是空行
		*p = '\0';                  // 收尾最后一个参数
		*(pa++) = last_p;
		*pa = 0;                    // 收尾最后一行,参数列表末尾用 null 标识列表结束
		run(argv[1], argsbuf);      // 执行最后一行指令
	}
	while(wait(0) != -1) {};        // 循环等待所有子进程完成，每一次 wait(0) 等待一个
	exit(0);
}