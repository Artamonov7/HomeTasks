#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "storage.h"

static volatile sig_atomic_t stop_reader = 0;

void signal_handler(int signum)
{
	stop_reader = 1;
}

int main()
{
	struct sigaction sa;
	struct shared_storage *ss = init_storage(0);

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);

	while (!stop_reader) {
		char buffer[64];
		/* tiger want random amount of food 1..10 */
		int bytes_to_read = rand() % 10 + 1;
		int len = storage_read(ss, buffer, bytes_to_read);
		if (len > 0) {
			printf("eat: %.*s\n", len, buffer);
			fflush(stdout);
			sleep(1); /* tiger sleeps :) */
		}
	}
	destroy_storage(ss);
	printf("Gracefully finished\n");
	return 0;
}
