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
}
