#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <memory.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <inttypes.h>

int main(void) {

    int fd = open("test.txt", O_RDWR | O_CREAT, 0666);
    ftruncate(fd, 256); // Ensure file is 256 bytes

    void* map = mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE, fd, 0);
    close(fd);

    // if (*((int*) map) == -1) {
    //     perror("Failed to map: ");
    // }

    uint8_t run = 1;
    char proc_end_cmd[256] = { 'e','n','d' };
    char data[256];

    while (run) {

        printf("Enter something> ");
        scanf("%255s", data);
        printf("\n");

        if (memcmp(data, proc_end_cmd, 3) == 0) run = 0;

        memcpy(
            map,
            (void*) data,
            256
        );

        msync(map, 256, MS_SYNC);
        
    }

    printf("Terminated!\n");

    return 0;

}