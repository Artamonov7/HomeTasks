#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "storage.h"

static volatile sig_atomic_t stop_writer = 0;

void signal_handler(int signum)
{
	stop_writer = 1;
}

int main()
{
	struct sigaction sa;
	struct shared_storage *ss = init_storage(1);

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);

	while (!stop_writer) {
		char buffer[256];
		if (!fgets(buffer, sizeof(buffer), stdin))
			break;
		storage_post(ss, buffer, strlen(buffer));
		printf("feed tiger...\n");
	}
	destroy_storage(ss);
	printf("Gracefully finished\n");
	return 0;
}
