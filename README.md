# Linux_GPIO is C++ class to work with GPIO in Linux over the filesystem


## Description

Linux_GPIO is C++ class to work with GPIO in Linux over the filesystem.


**The class has the following public methods:**
```C++
int  dev_open(unsigned int num_pin);
int  dev_open(unsigned int num_pin, GPIO_Direction direction);
void dev_close(void);

int  get_value(void);
int  up(void);
int  down(void);


unsigned int  get_num_pin(void){ return _num_pin; }
int           set_direction(GPIO_Direction direction);
int           get_direction(GPIO_Direction *direction);


GPIO_Error get_errno(void){ return _errno; }

static const char* strerror(GPIO_Error error);
```



**The class has the following public enums:**
```C++
enum GPIO_Error{

    ERROR_DEV_NOT_OPEN,
    ERROR_CANT_OPEN_DEV,
    ERROR_CANT_OPEN_EXPORT,
    ERROR_CANT_EXPORT,
    ERROR_CANT_OPEN_DIRECTION,
    ERROR_CANT_GET_DIRECTION,
    ERROR_CANT_SET_DIRECTION,
    ERROR_BAD_PARAM,
    ERROR_CANT_READ,
    ERROR_CANT_WRITE
};


enum GPIO_Direction{

    GPIO_IN,
    GPIO_OUT
};
```

More details see: **[linux_gpio.h](./linux_gpio.h)**

***
<br/>
## Usage

**To start working, perform the following steps:**

1. You need to include **[linux_gpio.h](./linux_gpio.h)** file in your **.cpp** file.
2. And add file **[linux_gpio.cpp](./linux_gpio.cpp)** to list of source files to compile. (see an example)

#### example:
```C++
int ret;
Linux_GPIO  gpio;

if( gpio.dev_open(gpio_pin, direct) != 0 )
{
    printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
    exit(-1);
}

ret = gpio.get_value();

if( ret == -1 )
{
    printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
    exit(-1);
}

printf("GPIO: %d  has value == %d\n", gpio_pin, ret);

gpio.dev_close();
```
More details can be found in the test application: **[linux_gpio_test.cpp](./linux_gpio_test.cpp)**


***
<br/>
## Test tool (**[linux_gpio_test](./linux_gpio_test.cpp)**)

Up GPIO 80:
```console
# ./linux_gpio_test -p 80 -u --direction 0
GPIO: 80  was Up
```

Down GPIO 80:
```console
# ./linux_gpio_test -p 80 -d --direction 0
GPIO: 80  was Down
```

Get value of GPIO 80:
```console
# ./linux_gpio_test -g 80
GPIO: 80  has value == 0
```

***
<br/>
## License

[BSD](./LICENSE).
