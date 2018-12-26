// system.cpp

#include <system.h>

#include <linux/reboot.h>
#include <unistd.h>
#include <sys/reboot.h>

void system::reboot()
{
    ::reboot(LINUX_REBOOT_CMD_RESTART);
}

void system::shutdown()
{
    ::reboot(LINUX_REBOOT_CMD_POWER_OFF);
}
