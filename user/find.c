#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *basename(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    // printf("get basename of %s: %s\n", path, p + 1);
    return ++p;
}

void concatpath(char *fullname, char *path, char *name)
{
    char *p;
    strcpy(fullname, path);
    p = fullname + strlen(fullname);
    *p++ = '/';
    memmove(p, name, strlen(name));
    *(p + strlen(name)) = 0;
    // printf("concat %s, %s to %s\n", path, name, fullname);
}

void find(char *path, const char *search)
{
    int fd;
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "cannot open file: %s\n", path);
        return;
    }

    struct stat st;
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "cannot stat file: %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE)
    {
        // printf("current: %s, searched: %s\n", path, search);
        if (strcmp(basename(path), search) == 0)
        {
            printf("%s\n", path);
        }
        close(fd);
        return;
    }

    if (st.type == T_DIR)
    {
        struct dirent de;
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
            {
                continue;
            }

            if (de.inum > 0)
            {
                // printf("path: %d, name: %d\n", path, de.name);
                char fullpath[512];
                concatpath(fullpath, path, de.name);
                find(fullpath, search);
            }
        }
        close(fd);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "please input dir and file name!\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
