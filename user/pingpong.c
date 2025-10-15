#include "kernel/types.h"
#include "user/user.h"

int main(){
    //创建两个管道 [0]读 [1]写
    int fd_parent[2];
    int fd_child[2];
    pipe(fd_parent);
    pipe(fd_child); 
    char buf[10];
    int pid = fork();
    if(pid<0){
        printf("fork failed\n");
        exit(-1);
    }
    if(pid==0){
        //子进程从父进程管道读，从子进程管道写
        close(fd_parent[1]);
        close(fd_child[0]);
        read(fd_parent[0],buf,4);//读取前4个字节
        printf("%d: received %s\n",getpid(),buf);
        write(fd_child[1],"pong",4);//写入前4个字节
    }
    if(pid>0){
        close(fd_parent[0]);
        close(fd_child[1]);
        write(fd_parent[1],"ping",4);//因为父进程先创建所以要先写，不然会死锁
        read(fd_child[0],buf,4);
        printf("%d: received %s\n",getpid(),buf);
    }
    exit(0);
    return 0;
}