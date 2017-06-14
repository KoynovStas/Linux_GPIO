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


static const char *short_opts = ":p:udghv?";
static const char *help_str   =
        " ===============  Help  ===============\n"
        " Test   name:  " TEST_NAME        "\n"
        " Test    ver:  " TEST_VERSION_STR "\n"
        " Build  date:  " __DATE__         "\n"
        " Build  time:  " __TIME__         "\n\n"
        "Options:                      description:\n\n"
        "  -u   --up                   GPIO Up\n"
        "  -d   --down                 GPIO Down\n"
        "  -p   --pin                  Set number pin\n"
        "  -g   --get                  Get value GPIO\n"
        "       --direction            Set direction for GPIO: 0 is Out\n"
        "                                                      1 is In\n"
        "                                                     -1 not change\n"
        "  -v   --version              Display test version information\n"
        "  -h,  --help                 Display this information\n\n";



static const struct option long_opts[] = {
    { "up",       no_argument,       NULL, 'u' },
    { "down",     no_argument,       NULL, 'd' },
    { "get",      no_argument,       NULL, 'g' },
    { "pin",      required_argument, NULL, 'p' },
    { "direction",required_argument, NULL,  0  },
    { "version",  no_argument,       NULL, 'v' },
    { "help",     no_argument,       NULL, 'h' },
    { NULL,       no_argument,       NULL,  0  }
};





void processing_cmd(int argc, char *argv[])
{

    int opt, long_index;



    opt = getopt_long(argc, argv, short_opts, long_opts, &long_index);
    while( opt != -1 )
    {
        switch( opt )
        {

            case 'u':
                        action = Up;
                        break;

            case 'd':
                        action = Down;
                        break;

            case 'g':
                        action = Get;
                        break;

            case 'p':
                        gpio_pin = atoi(optarg);
                        break;

            case 'v':
                        puts(TEST_NAME "  version  " TEST_VERSION_STR "\n");
                        exit(EXIT_SUCCESS);
                        break;

            case 'h':

                        puts(help_str);
                        exit(EXIT_SUCCESS);
                        break;
            case '?':
                        printf("Unsupported option: -%c see help\n", optopt);
                        exit(EXIT_SUCCESS);
                        break;
            case ':':
                        printf("Option -%c requires an operand see help\n", optopt);
                        exit(EXIT_SUCCESS);
                        break;


            case 0:     // long options


                  if( strcmp( "direction", long_opts[long_index].name ) == 0 )
                  {
                      direction = atoi(optarg);
                      break;
                  }


            default:
                  break;
        }

        opt = getopt_long(argc, argv, short_opts, long_opts, &long_index);
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
