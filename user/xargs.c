#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char whitespace[] = " \t\r\n\v";

// read args from standar input
int getarges(char *buf, int nbuf)
{
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if (buf[0] == 0)
    {
        return -1;
    }
    return 0;
}

void parseargs(char *buf, char **args, int offset)
{
    args += offset;
    int argc = MAXARG - offset;
    memset(args, 0, argc * sizeof(char *));

    char *es = buf + strlen(buf);
    while (buf != es)
    {
        while (buf < es && strchr(whitespace, *buf))
            buf++;
        *args = buf;
        args++;
        while (buf < es && !strchr(whitespace, *buf))
            buf++;
        if (buf < es)
        {
            *buf = 0;
            buf++;
        }
    }
}

void checkargs(char **args)
{
    int i;
    for (i = 0; i < MAXARG; i++)
    {
        printf("arg%d: %s\n", i, *args);
        args++;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        fprintf(2, "please input cmd\n");
        exit(1);
    }
    char *args[MAXARG];
    int i;
    for (i = 1; i < argc; i++)
    {
        args[i - 1] = argv[i];
    }
    char buf[100];
    while (getarges(buf, sizeof(buf)) == 0)
    {
        if (fork() == 0)
        {
            parseargs(buf, args, argc - 1);
            exec(argv[1], args);
        }
        wait(0);
    }
    exit(0);
}
