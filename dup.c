#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    int pd[2];
    if (pipe(pd) < 0)
        exit(1);

    if (fork() == 0) {
        close(pd[0]);
        dup2(pd[1], 1);
        execl(argv[1], argv[1], NULL);
        exit(1);
    }
    else {
        close(pd[1]);
        char longBuffer[4096];
        wait(NULL);

        int new_lines = 0;
        int i = 0;

        int count = read(pd[0], longBuffer, 4096);

        while (i < count) {
            while (new_lines < atoi(argv[2]) && i < count) {
                printf("%c", longBuffer[i]);

                if (longBuffer[i] == '\n')
                    new_lines += 1;
                i += 1;
            }
            
            printf("--MORE--");
            new_lines = 0;
            getchar();
            system("clear");
        }
    }

    return 0;
}