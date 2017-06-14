#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>



#include "linux_gpio.h"



#define TEST_MAJOR_VERSION  1
#define TEST_MINOR_VERSION  0
#define TEST_PATCH_VERSION  0





#define TEST_DEF_TO_STR_(text) #text
#define TEST_DEF_TO_STR(arg) TEST_DEF_TO_STR_(arg)


#define TEST_MAJOR_VERSION_STR  TEST_DEF_TO_STR(TEST_MAJOR_VERSION)
#define TEST_MINOR_VERSION_STR  TEST_DEF_TO_STR(TEST_MINOR_VERSION)
#define TEST_PATCH_VERSION_STR  TEST_DEF_TO_STR(TEST_PATCH_VERSION)

#define TEST_VERSION_STR  TEST_MAJOR_VERSION_STR "." \
                          TEST_MINOR_VERSION_STR "." \
                          TEST_PATCH_VERSION_STR




enum TEST_Action {
    Up,
    Down,
    Get
};


unsigned int gpio_pin = 0;
TEST_Action  action   = Get;
int direction         = -1; //not change



static const char *help_str   =
        " ===============  Help  ===============\n"
        " Test   name:  " TEST_NAME        "\n"
        " Test    ver:  " TEST_VERSION_STR "\n"
        " Build  date:  " __DATE__         "\n"
        " Build  time:  " __TIME__         "\n\n"
        "Options:                    description:\n\n"
        "     --up                   GPIO Up\n"
        "     --down                 GPIO Down\n"
        "     --pin         [value]  Set number pin\n"
        "     --get                  Get value GPIO\n"
        "     --direction   [value]  Set direction for GPIO: 0 is Out\n"
        "                                                    1 is In\n"
        "                                                   -1 not change\n"
        "  -v  --version             Display test version information\n"
        "  -h  --help                Display this information\n\n";





// indexes for long_opt function
enum
{
    cmd_opt_help    = 'h',
    cmd_opt_version = 'v',

    cmd_opt_up,
    cmd_opt_down,
    cmd_opt_pin,
    cmd_opt_get,
    cmd_opt_direction
};



static const char *short_opts = "hv";

static const struct option long_opts[] =
{
    { "version",  no_argument,       NULL, cmd_opt_version   },
    { "help",     no_argument,       NULL, cmd_opt_help      },

    { "up",       no_argument,       NULL, cmd_opt_up        },
    { "down",     no_argument,       NULL, cmd_opt_down      },
    { "pin",      required_argument, NULL, cmd_opt_pin       },
    { "get",      no_argument,       NULL, cmd_opt_get       },
    { "direction",required_argument, NULL, cmd_opt_direction },

    { NULL,       no_argument,       NULL, 0                 }
};





void processing_cmd(int argc, char *argv[])
{
    int opt;


    while( (opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1 )
    {
        switch( opt )
        {

            case cmd_opt_help:
                        puts(help_str);
                        exit(EXIT_SUCCESS);
                        break;

            case cmd_opt_version:
                        puts(TEST_NAME "  version  " TEST_VERSION_STR "\n");
                        exit(EXIT_SUCCESS);
                        break;

            case cmd_opt_up:
                        action = Up;
                        break;

            case cmd_opt_down:
                        action = Down;
                        break;

            case cmd_opt_pin:
                        gpio_pin = atoi(optarg);
                        break;

            case cmd_opt_get:
                        action = Get;
                        break;

            case cmd_opt_direction:
                        direction = atoi(optarg);
                        break;

            default:
                        puts("for more detail see help\n\n");
                        exit(EXIT_FAILURE);
                        break;
        }
    }
}



void run_test()
{
    int ret;


    Linux_GPIO  gpio;


    if( gpio.dev_open(gpio_pin) != 0 )
    {
        printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
        exit(-1);
    }


    if( direction == 0 )
    {
        ret = gpio.set_direction(Linux_GPIO::GPIO_OUT);

        if( ret == -1 )
        {
            printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
            exit(-1);
        }

        printf("GPIO: %d  Set direction OUT\n", gpio_pin);

    }


    if( direction == 1 )
    {
        ret = gpio.set_direction(Linux_GPIO::GPIO_IN);

        if( ret == -1 )
        {
            printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
            exit(-1);
        }

        printf("GPIO: %d  Set direction IN\n", gpio_pin);

    }



    if( action == Get )
    {
        ret = gpio.get_value();

        if( ret == -1 )
        {
            printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
            exit(-1);
        }

        printf("GPIO: %d  has value == %d\n", gpio_pin, ret);
    }


    if( action == Up )
    {
        ret = gpio.up();

        if( ret == -1 )
        {
            printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
            exit(-1);
        }

        printf("GPIO: %d  was Up\n", gpio_pin);
    }



    if( action == Down )
    {
        ret = gpio.down();

        if( ret == -1 )
        {
            printf("Error: %s\n", gpio.strerror(gpio.get_errno()));
            exit(-1);
        }

        printf("GPIO: %d  was Down\n", gpio_pin);
    }

}



int main(int argc, char *argv[])
{

    processing_cmd(argc, argv);

    run_test();

    return EXIT_SUCCESS; // good job
}
