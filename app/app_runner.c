#include "app_device.h"
#include "unistd.h"
#include "stdlib.h"
#include "app_bt.h"
#include "signal.h"
#include "app_runner.h"

static int is_running = 1;
static void runner_exit(int sig)
{
    is_running = 0;
}

int app_runner_init(void)
{
    signal(SIGINT, runner_exit);
    signal(SIGTERM, runner_exit);
    Device *device = app_device_init(DEVICE_FILE);
    app_bt_init(device);
    app_device_start(device);
    while (is_running) {
        sleep(1);
    }
    app_device_close();
}