#include "meltdown.h"

static jmp_buf jump_buffer;

static void unblock_signal(int signal_number __attribute__((__unused__))) {
  sigset_t signal_set;
  sigemptyset(&signal_set);
  sigaddset(&signal_set, signal_number);
  sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
}

static void segfault_handler(int signal_number) {
  (void)signal_number;
  unblock_signal(SIGSEGV);
  longjmp(jump_buffer, 1);
}

int main(){
	int arr[10];
	signal(SIGSEGV, segfault_handler);
	if(!setjmp(jump_buffer)){
		int x = arr[1000000];
	}
	printf("Success\n");
	printf("I did it. Hell yeah!\n");
}