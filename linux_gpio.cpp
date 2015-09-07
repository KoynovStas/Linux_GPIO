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
    _num_pin(0)
{
}



Linux_GPIO::~Linux_GPIO()
{
    dev_close();
}



int Linux_GPIO::dev_open(unsigned int num_pin)
{
    if( gpio_open(num_pin) == 0 )
        return 0; //good job dev is open


    // Cant open dev for GPIO (num_pin)
    // The file system have no file our GPIO
    // we must to export to the file system
    if( gpio_export(num_pin) != 0 )
        return -1;  //cant export


    // was export to the file system
    // reopen dev
    return gpio_open(num_pin);
}



int Linux_GPIO::dev_open(unsigned int num_pin, Linux_GPIO::GPIO_Direction direction)
{

    if( dev_open(num_pin) != 0 )
        return -1; //cant open dev


    if( set_direction(direction) != 0 )
    {
        dev_close();
        return -1;  //cant set direction
    }


    return 0; //good job
}



void Linux_GPIO::dev_close()
{

    if( _dev_fd != -1 )
        close(_dev_fd);



    _errno     = ERROR_DEV_NOT_OPEN;
    _dev_fd    = -1;
    _num_pin   =  0;
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



int Linux_GPIO::set_direction(Linux_GPIO::GPIO_Direction direction)
{
    int  fd, ret, right_ret;
    char temp_buf[100];


    if( _dev_fd == -1 )
        return -1;  //dev not open


    // set the direction of the gpio_pin
    sprintf(temp_buf, "/sys/class/gpio/gpio%d/direction", _num_pin);
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



int Linux_GPIO::get_direction(GPIO_Direction *direction)
{
    int  fd, ret;
    char temp_buf[100];


    if( !direction )
    {
        _errno = ERROR_INVALID_PARAM;
        return -1;
    }


    if( _dev_fd == -1 )
        return -1;  //dev not open



    // the direction of the gpio_pin
    sprintf(temp_buf, "/sys/class/gpio/gpio%d/direction", _num_pin);
    fd = open(temp_buf, O_RDONLY);


    if( fd == -1 )
    {
        _errno = ERROR_CANT_OPEN_DIRECTION;
        return -1;
    }


    ret = read(fd, temp_buf, 4);
    close(fd);


    if( ret <= 0 )
    {
        _errno = ERROR_CANT_GET_DIRECTION;
        return -1;
    }


    if( strcmp( "in", temp_buf ) == 0 )
        *direction = GPIO_IN;
    else
        *direction = GPIO_OUT;


    return 0; //good job
}



const char* Linux_GPIO::strerror(GPIO_Error error)
{

    switch (error)
    {
        case ERROR_INVALID_PARAM:
            return "invalid param";

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

        case ERROR_CANT_GET_DIRECTION:
            return "cant get direct";

        case ERROR_CANT_SET_DIRECTION:
            return "cant set direct";

        case ERROR_CANT_READ:
            return "cant read /sys/class/gpio/gpioX/value";

        case ERROR_CANT_WRITE:
            return "cant write to /sys/class/gpio/gpioX/value";

        default:
            return "unknown error";
    }
}



int Linux_GPIO::gpio_open(unsigned int num_pin)
{
    char temp_buf[100];


    dev_close(); // close old dev


    sprintf(temp_buf, "/sys/class/gpio/gpio%d/value", num_pin);

    _dev_fd = open(temp_buf, O_RDWR);

    if( _dev_fd == -1 )
    {
        _errno = ERROR_CANT_OPEN_DEV;
        return -1;
    }


    _num_pin = num_pin;


    return 0; //good job
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
