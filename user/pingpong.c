#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    if (fork() == 0)
    {
        // child process
        // receive ping
        close(p1[1]);
        char rev[1];
        if (read(p1[0], rev, sizeof rev) == 0)
        {
            fprintf(2, "%s: receive ping error!\n", getpid());
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        close(p1[0]);
        // send pong
        close(p2[0]);
        const char pong[1] = {0};
        if (write(p2[1], pong, sizeof pong) == 0)
        {
            fprintf(2, "%s: send pong error!\n", getpid());
            exit(1);
        }
        close(p2[1]);
    }
    else
    {
        // parent process
        // send ping
        close(p1[0]);
        const char ping[1] = {0};
        if (write(p1[1], ping, sizeof ping) == 0)
        {
            fprintf(2, "%s: send ping error!\n", getpid());
            exit(1);
        }
        close(p1[1]);
        // receive pong
        close(p2[1]);
        char rev[1];
        if (read(p2[0], rev, sizeof rev) == 0)
        {
            fprintf(2, "receive pong error!\n", getpid());
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(p2[0]);
    }
    exit(0);
}
