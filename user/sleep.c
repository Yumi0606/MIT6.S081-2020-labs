#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(2, "Usage: sleep <seconds>\n"); // 2 = stderr
        exit(1);
    }
    int i;
    i = atoi(argv[1]);
    if (i > 0)
    {
        sleep(i);
    }
    else
    {
        fprintf(2, "Error: sleep time must be a positive integer\n");
        exit(1);
    }
    exit(0);
}
