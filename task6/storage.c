#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>          
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include "storage.h"

struct shared_storage *init_storage(int is_writer)
{
	struct shared_storage *ss = malloc(sizeof(struct shared_storage));
	int fd = shm_open(SHMEM_STORAGE_FILE, O_RDWR | O_CREAT, 0644);
	if (fd < 0) {
		perror("open " SHMEM_STORAGE_FILE);
		exit(1);
	}

	if (is_writer)
		ftruncate(fd, SHMEM_STORAGE_SIZE);

	void *memptr = mmap(NULL, SHMEM_STORAGE_SIZE,
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (memptr == (void *)-1) {
		perror("mmap");
		exit(1);
	}

	sem_t *semr = sem_open(SEMAPHORE_READ, O_CREAT, 0644, 0);
	if (semr == (void *)-1) {
		perror("sem_open");
		exit(1);
	}
	sem_t *semw = sem_open(SEMAPHORE_WRITE, O_CREAT, 0644, 1);
	if (semw == (void *)-1) {
		perror("sem_open");
		exit(1);
	}

	ss->fd = fd;
	ss->memptr = memptr;
	ss->wait_read = semr;
	ss->wait_write = semw;

	ss->read_pos = -1;
	ss->mesg_len = 0;
	ss->is_writer = is_writer;
	return ss;
}

void destroy_storage(struct shared_storage *ss)
{
	munmap(ss->memptr, SHMEM_STORAGE_SIZE);
	close(ss->fd);
	sem_close(ss->wait_read);
	sem_close(ss->wait_write);

	shm_unlink(SHMEM_STORAGE_FILE);
	sem_unlink(SEMAPHORE_READ);
	sem_unlink(SEMAPHORE_WRITE);
	free(ss);
}

void storage_post(struct shared_storage *ss, const void *buf, int len)
{
	if (!sem_wait(ss->wait_write)) {
		memcpy(ss->memptr, &len, sizeof(len));
		memcpy(ss->memptr + sizeof(len), buf, len);
		sem_post(ss->wait_read);
	}
}

int storage_read(struct shared_storage *ss, void *buf, int len)
{
	if (ss->read_pos == -1) {
		if (!sem_wait(ss->wait_read)) {
			memcpy(&ss->mesg_len, ss->memptr, sizeof(int));
			ss->read_pos = 0;
		} else {
			return -1;
		}
	}
	if (len > ss->mesg_len - ss->read_pos)
		len = ss->mesg_len - ss->read_pos;
	memcpy(buf, ss->memptr + sizeof(ss->mesg_len) + ss->read_pos, len);
	ss->read_pos += len;
	if (ss->read_pos >= ss->mesg_len) {
		sem_post(ss->wait_write);
		ss->read_pos = -1;
		ss->mesg_len = 0;
	}
	return len;
}
