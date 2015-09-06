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
    _dev_fd(-1),
    _num_pin(0),
    _direction(GPIO_IN)
{
}



Linux_GPIO::~Linux_GPIO()
{
    dev_close();
}



int Linux_GPIO::dev_open(unsigned int num_pin, Linux_GPIO::GPIO_Direction direction)
{
    char temp_buf[100];


    dev_close(); // close old dev


    if( gpio_export(num_pin) != 0 )
        return -1;  //cant export



    if( set_direction(num_pin, direction) != 0 )
        return -1;  //cant set direction



    sprintf(temp_buf, "/sys/class/gpio/gpio%d/value", num_pin);

    _dev_fd = open(temp_buf, O_RDWR);

    if( _dev_fd == -1 )
    {
        _errno = ERROR_CANT_OPEN_DEV;
        return -1;
    }


    _num_pin   = num_pin;
    _direction = direction;


    return 0; //good job
}



void Linux_GPIO::dev_close()
{

    if( _dev_fd != -1 )
        close(_dev_fd);



    _errno     = ERROR_DEV_NOT_OPEN;
    _dev_fd    = -1;
    _num_pin   =  0;
    _direction = GPIO_IN;
}



int Linux_GPIO::get_value()
{
    int  ret;
    char val = 0;

    if( _dev_fd == -1 )
        return -1;


    lseek(_dev_fd, 0, SEEK_SET);
    ret = read(_dev_fd, &val, 1);

    if( ret != 1 )
    {
        _errno = ERROR_CANT_READ;
        return -1;
    }


    return (val == '1');   //return 1 if GPIO is high, if GPIO is low return 0
}



int Linux_GPIO::up()
{
    int ret;

    if( _dev_fd == -1 )
        return -1;


    ret = write(_dev_fd, "1", 2);

    if( ret != 2 )
    {
        _errno = ERROR_CANT_WRITE;
        return -1;
    }


    return 0; //good job
}



int Linux_GPIO::down()
{
    int ret;

    if( _dev_fd == -1 )
        return -1;


    ret = write(_dev_fd, "0", 2);

    if( ret != 2 )
    {
        _errno = ERROR_CANT_WRITE;
        return -1;
    }


    return 0; //good job
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

        case ERROR_CANT_EXPORT:
            return "cant export pin";

        case ERROR_CANT_OPEN_DIRECTION:
            return "cant open /sys/class/gpio/gpioX/direction";

        case ERROR_CANT_SET_DIRECTION:
            return "cant set direction";

        case ERROR_CANT_READ:
            return "cant read /sys/class/gpio/gpioX/value";

        case ERROR_CANT_WRITE:
            return "cant write to /sys/class/gpio/gpioX/value";

        default:
            return "unknown error";
    }
}



int Linux_GPIO::gpio_export(unsigned int num_pin)
{
    int  fd, ret;
    char temp_buf[100];


    //To export specified GPIO (gpio_pin) to file system
    fd = open("/sys/class/gpio/export", O_WRONLY);

    if( fd == -1 )
    {
        _errno = ERROR_CANT_OPEN_EXPORT;
        return -1;
    }


    sprintf(temp_buf, "%d", num_pin);
    ret = write(fd, temp_buf, strlen(temp_buf));
    close(fd);

    if( ret != (int)strlen(temp_buf) )
    {
        _errno = ERROR_CANT_EXPORT;
        return -1;
    }


    return 0; //good job
}



int Linux_GPIO::set_direction(unsigned int num_pin, Linux_GPIO::GPIO_Direction direction)
{
    int  fd, ret, right_ret;
    char temp_buf[100];


    // set the direction of the gpio_pin
    sprintf(temp_buf, "/sys/class/gpio/gpio%d/direction", num_pin);
    fd = open(temp_buf, O_WRONLY);


    if( fd == -1 )
    {
        _errno = ERROR_CANT_OPEN_DIRECTION;
        return -1;
    }


    if( direction == GPIO_IN )
    {
        ret = write(fd, "in", 3);
        right_ret = 3;
    }
    else
    {
        ret = write(fd, "out", 4);
        right_ret = 4;
    }

    close(fd);


    if( ret != right_ret )
    {
        _errno = ERROR_CANT_SET_DIRECTION;
        return -1;
    }


    return 0; //good job
}
