#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path) //参考ls中的fmtname代码
{
	static char buf[DIRSIZ+1];
 	char *p;
  	// Find first character after last slash.
  	for(p=path+strlen(path); p >= path && *p != '/'; p--)
    	;
  	p++;
  	// Return blank-padded name.
  	if(strlen(p) >= DIRSIZ)
    	return p;
  	memmove(buf, p, strlen(p));
	buf[strlen(p)] = 0;  //字符串结束符
  	return buf;
}



void find(char *path,char *name)
{
  char buf[512], *p;   //p指针指向buf
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

  switch(st.type){
  case T_FILE:
    if(strcmp(fmtname(path),name) == 0)
    printf("%s\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
        
      memmove(p, de.name, DIRSIZ);  //复制DIRSIZ大小的文件名到p指针指向的位置
      p[DIRSIZ] = 0;   //确保复制的字符串以null结束
      
      if(!strcmp(de.name, ".") || !strcmp(de.name, ".."))   //
      	continue;
      find(buf,name);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc!=3){
  fprintf(2, "usage:find <path> <name>\n");
  exit(1);
  }
  find(argv[1],argv[2]);   //find(. b)//寻找当前路径下的b文件
  exit(0);
}