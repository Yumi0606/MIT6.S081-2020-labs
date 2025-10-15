#include "kernel/types.h"
#include "user/user.h"//注意顺序
#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/fs.h"


int main(int argc, char *argv[])
{
  char buf[512];
  char* full_argv[MAXARG];
  int i;
  int len;
  if(argc < 2){
    fprintf(2, "usage: xargs your_command\n");
    exit(1);
  }

  if (argc + 1 > MAXARG) {
      fprintf(2, "too many args\n");
      exit(1);
  }

   //echo hello too | xargs echo bye
   //echo hello too会将echo bye作为标准输入 作为命令行参数传给 xargs echo bye
   //程序可以用read（） 中buf存取 hello too
  for (i = 1; i < argc; i++) {
      full_argv[i-1] = argv[i];
  }
   //以'\0'结尾
  full_argv[argc] = 0;
  
  while (1) {
      i = 0;
      while (1) {
        len = read(0,&buf[i],1); //buf里面存取 标准输入 hello too
        if (len == 0 || buf[i] == '\n') break;    //将hello too 读入buf
        i++;
      }
      if (i == 0) break;
      buf[i] = 0;//以'\0'结尾
      full_argv[argc-1] = buf;
      if (fork() == 0) {
        // fork a child process to do the job
        exec(full_argv[0],full_argv);    //规则就是这样
        exit(0);
      } else {
        // 等待子进程完成
        wait(0);
      }
  }
  exit(0);
}