#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <memory.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/eventfd.h>

int main(void) {

    int fd = open("test.txt", O_RDWR);

    void* map = mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE, fd, 0);
    close(fd);

    // if (*((int*) map) == -1) {
    //     perror("Failed to map: ");
    // }

    uint8_t run = 1;
    const char proc_end_cmd[256] = { 'e','n','d' };

    while (run) {

         sleep(3); // simple polling

        if (memcmp(map, proc_end_cmd, 3) == 0) run = 0;
        fprintf(stdout, "Received message: %s\n", (char*) map);

    }

    printf("Terminated!\n");

    return 0;

}