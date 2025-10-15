#include "kernel/types.h"
#include "user/user.h"
#define N 35
// 程序使用一个主进程和多个solve()进程，每个进程会筛除掉pr中各自代表的素数的倍数
// pr放进程被筛选出来的质数（输出答案），会被多次读取和存入
// pl开启子进程，作为初始输入
void solve(int pl[])
{
    int prime;
    if (read(pl[0], &prime, sizeof(prime)) == 0)
        return;
    printf("prime %d\n", prime);
    int pr[2];
    pipe(pr);

    int pid = fork();
    if (pid > 0)
    {
        // 父进程筛去当前质数的倍数，把剩下的数写入pr管道
        close(pr[0]);
        for (int i; read(pl[0], &i, sizeof(i)) != 0;)
        {
            if (i % prime != 0)
            {
                write(pr[1], &i, sizeof(i));
            }
        }
        close(pr[1]);
        close(pl[0]);
        wait(0);
        exit(0);
    }
    else if (pid == 0)
    {
        // 子进程继续solve()处理pr管道剩下的质数
        close(pl[0]);
        close(pr[1]);
        solve(pr);
        close(pr[0]);
        exit(0);
    }
    else
    {
        // 创建进程失败，关闭管道
        close(pl[0]);
        close(pr[1]);
        close(pr[0]);
        exit(-1);
    }
}

int main(int argc, char argv[])
{
    int pl[2];
    pipe(pl);
    int pid = fork();
    if (pid > 0)
    {
        close(pl[0]);
        for (int i = 2; i <= N; i++)
        {
            write(pl[1], &i, sizeof(i));
        }
        close(pl[1]);
        wait(0);
        exit(0);
    }
    else if (pid == 0)
    {
        close(pl[1]);
        solve(pl);
        close(pl[0]);
        exit(0);
    }
    else
    {
        close(pl[1]);
        close(pl[0]);
        exit(-1);
    }
    return 0;
}