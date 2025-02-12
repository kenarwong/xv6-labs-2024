#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char * getname(char* path)  //"fmt" stands for "format"
{
  char *p;
  for (p=path+strlen(path); p>=path && *p!='/'; p--)
    ;
  p++;

  return p;
}

void find(char *path , char *file)
{
  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd=open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot fstat %s\n", path);
  }

  switch (st.type) {
  case T_DEVICE:
  case T_FILE:
    if((strcmp(getname(path),getname(file))) == 0)
      printf("%s\n", path);
    break;
  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      fprintf(2, "find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if((strcmp(de.name,".")) == 0 || (strcmp(de.name,"..")) == 0) {
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      find(buf,file);
    }
    break;
   }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: find [path] file\n");
    exit(1);
  }

  find(argv[1],argv[2]);

  exit(0);
}