#include "app_runner.h"
#include "log/log.h"
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        log_error("No arguments provided");
        return -1;

    }
    if (strcmp(argv[1], "app") == 0)
    {
        app_runner();
    }else{
        log_error("Invalid argument");
        return -1;
    }
    return 0;
}
