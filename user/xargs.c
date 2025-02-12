#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char *cmd;
  if (argc < 2) {
    cmd = "echo";
  } else {
    cmd = argv[1];
  }

  char *args[MAXARG] = {[0] = cmd};

  int i;
  for (i = 2; i < (argc); ++i) {
    args[i-1] = argv[i];
  }
  --i;

  char buf[512];
  char *p = buf;
  char c;

  while (((read(0, &c, sizeof(char))) > 0))
  {
    if (c != '\n') {
      *p++ = c;
    } else {
      *p = '\0';
      p = buf;

      if(fork() == 0) {
        args[i] = buf;
        exec(cmd, args);
        fprintf(2, "exec %s failed\n", cmd);
      } else {
        wait(0);
        memset(buf, 0, sizeof(buf));
      }
    }
  }

  exit(0);
}