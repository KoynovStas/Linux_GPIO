#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#include "linux_gpio.h"






Linux_GPIO::Linux_GPIO() :
    _errno(ERROR_DEV_NOT_OPEN),
    _dev_fd(-1)
{
}



Linux_GPIO::~Linux_GPIO()
{
    dev_close();
}



void Linux_GPIO::dev_close()
{

    if( _dev_fd != -1 )
        close(_dev_fd);


    _dev_fd = -1;
    _errno  = ERROR_DEV_NOT_OPEN;
}



const char *Linux_GPIO::strerror(GPIO_Error errno)
{

    switch (errno)
    {
        case ERROR_DEV_NOT_OPEN:
            return "device is not open";

        case ERROR_CANT_OPEN_DEV:
            return "cant open device";

        case ERROR_CANT_OPEN_EXPORT:
            return "cant open /sys/class/gpio/export";

        case ERROR_CANT_OPEN_DIRECTION:
            return "cant open /sys/class/gpio/gpioX/direction";

        case ERROR_CANT_READ:
            return "cant read /sys/class/gpio/gpioX/value";

        case ERROR_CANT_WRITE:
            return "cant write to /sys/class/gpio/gpioX/value";

        default:
            return "unknown error";
    }
}
