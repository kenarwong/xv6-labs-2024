#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];

  pipe(p1);
  pipe(p2);

  if (fork() == 0) {
    char buf[1];
    close(p1[1]);
    close(p2[0]);

    read(p1[0], buf, sizeof buf);
    printf("%d: received ping\n", getpid());
    close(p1[0]);

    write(p2[1], "p", 1);
    close(p2[1]);
    
  } else {
    char buf[1];
    close(p1[0]);
    close(p2[1]);

    write(p1[1], "p", 1);
    close(p1[1]);

    read(p2[0], buf, sizeof buf);
    printf("%d: received pong\n", getpid());
    close(p2[0]);
  }

  exit(0);
}