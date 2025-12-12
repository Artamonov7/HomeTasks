#ifndef STORAGE_H_SENTRY
#define STORAGE_H_SENTRY

#include <semaphore.h>

#define SHMEM_STORAGE_FILE "storage.mem"
#define SHMEM_STORAGE_SIZE 4096

#define SEMAPHORE_READ "storage_read"
#define SEMAPHORE_WRITE "storage_write"

struct shared_storage {
	int fd;
	void *memptr;
	sem_t *wait_read;
	sem_t *wait_write;
	int read_pos;
	int mesg_len;
	int is_writer;
};

struct shared_storage *init_storage(int is_writer);

void destroy_storage(struct shared_storage *ss);

void storage_post(struct shared_storage *ss, const void *buf, int len);

int storage_read(struct shared_storage *ss, void *buf, int len);

#endif /* STORAGE_H_SENTRY */
