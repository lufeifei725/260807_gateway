#include "log/log.h"

int main(int argc, char const *argv[])
{
    // log_set_level(LOG_DEBUG);
    log_trace("This is a trace log");
    log_debug("This is a debug log");
    log_info("This is a info log");
    log_warn("This is a warn log");
    log_error("This is a error log");
    log_fatal("This is a fatal log");
    return 0;
}
