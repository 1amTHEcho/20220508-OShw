#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv) {
   
    int pp2c[2], pc2p[2];
    pipe(pp2c);
    pipe(pc2p);
    
    if(fork() != 0) { 
        // 1. 父进程首先向发出该字节
        write(pp2c[1], "!", 1); 
        char buf;
        // 2. 父进程发送完成后，开始等待子进程的回复
        read(pc2p[0], &buf, 1); 
        // 5. 子进程收到数据，read 返回，输出 pong
        printf("%d: received pong\n", getpid()); 
        wait(0);
    } else {
        char buf;
        // 3. 子进程读取管道，收到父进程发送的字节数据
        read(pp2c[0], &buf, 1); 
        printf("%d: received ping\n", getpid());
        // 4. 子进程通过 子->父 管道，将字节送回父进程
        write(pc2p[1], &buf, 1); 
    }
    exit(0);
}