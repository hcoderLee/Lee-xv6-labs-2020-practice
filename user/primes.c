#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void pfilter(int *p, int start)
{
    close(*(p + 1));
    int *num = malloc(sizeof(int));
    int p1[2];
    int initP = 0;

    printf("prime %d\n", start);

    while (read(*p, num, sizeof(int)) != 0)
    {
        // printf("pid: %d, level: %d, read: %d\n", getpid(), level, *num);
        if (*num % start == 0)
        {
            // num is not prime
            continue;
        }

        // hand num to right neighbor of pipline
        if (initP == 0)
        {
            initP = 1;
            if (pipe(p1) != 0)
            {
                fprintf(2, "process %d: create pip error!\n", getpid());
                exit(1);
            }
            // fork another process
            if (fork() == 0)
            {
                pfilter(p1, *num);
                exit(0);
            }
            else
            {
                close(p1[0]);
            }
        }
        // printf("pid: %d, level: %d, write: %d\n", getpid(), level, *num);
        write(p1[1], num, sizeof(int));
    }
    // printf("pid: %d, level: %d, read end\n", getpid(), level);
    close(*p);
    free(num);
    if (initP != 0)
    {
        close(p1[1]);
    }
    wait(0);
}

int main(int argc, char const *argv[])
{
    const int start = 2;
    int p[2];
    int initP = 0;
    int i;

    printf("prime %d\n", start);
    for (i = start; i <= 35; i++)
    {
        if (i % start == 0)
        {
            continue;
        }

        if (initP == 0)
        {
            initP = 1;
            if (pipe(p) != 0)
            {
                fprintf(2, "main process create pip error!\n");
                exit(1);
            }
            if (fork() == 0)
            {
                pfilter(p, i);
                exit(0);
            }
            else
            {
                close(p[0]);
            }
        }
        if (write(p[1], &i, sizeof(int)) != sizeof(int))
        {
            fprintf(2, "main proess write %d error!\n", i);
            exit(1);
        }
        // printf("pid: %d, level: %d, write: %d\n", getpid(), level, i);
    }
    // printf("finish loop\n");
    if (initP != 0)
    {
        // printf("pid: %d, close write end\n", getpid());
        close(p[1]);
    }
    wait(0);
    // printf("exit main process\n");
    exit(0);
}