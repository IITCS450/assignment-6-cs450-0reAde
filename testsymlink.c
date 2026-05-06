#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  int fd;
  char buf[64];

  // Test 1: read through a symlink
  fd = open("realfile", O_CREATE | O_WRONLY);
  write(fd, "hello", 5);
  close(fd);

  symlink("realfile", "mylink");

  fd = open("mylink", O_RDONLY);
  if(fd < 0){
    printf(1, "FAIL: could not open symlink\n");
    exit();
  }
  read(fd, buf, 5);
  buf[5] = '\0';
  close(fd);

  if(strcmp(buf, "hello") == 0)
    printf(1, "TEST 1 PASSED: read through symlink works\n");
  else
    printf(1, "TEST 1 FAILED: got %s\n", buf);

  // Test 2: cycle detection
  symlink("linkB", "linkA");
  symlink("linkA", "linkB");

  fd = open("linkA", O_RDONLY);
  if(fd < 0)
    printf(1, "TEST 2 PASSED: cycle correctly detected\n");
  else
    printf(1, "TEST 2 FAILED: cycle not detected\n");

  exit();
}
