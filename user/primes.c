#include "kernel/types.h"
#include "user/user.h"
 
void
primes(int p[2]) {
  int val;

  read(p[0], &val, sizeof (int));
  printf("prime %d\n", val);

  int pp[2];
  pipe(pp);

  int nextval = -1;
  while((read(p[0], &nextval, sizeof (int))) > 0) {
    if(nextval%val != 0) {
      write(pp[1], &nextval, sizeof (int));
    }
  }

  if (nextval == -1) {
    close(pp[0]);
    close(pp[1]);
    close(p[0]);
    return;
  }

  if (fork() == 0) {
    close(pp[1]);
    close(p[0]);

    primes(pp);
    close(pp[0]);

    exit(0);
  } else {
    close(pp[1]);
    wait(0);
  }
}

int
main(int argc, char *argv[])
{
  int n = 130;
  int p[2];
  pipe(p);

  for(int i=2; i<n; i++){
    int val = i;
    write(p[1], &val, sizeof (int));
  }

  close(p[1]);
  primes(p);
  close(p[0]);

  exit(0);
}