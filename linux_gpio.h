#ifndef LINUX_GPIO_H
#define LINUX_GPIO_H





class Linux_GPIO
{    

    public:


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


         Linux_GPIO();
        ~Linux_GPIO();


        int  dev_open(unsigned int num_pin);
        int  dev_open(unsigned int num_pin, GPIO_Direction direction);
        void dev_close(void);

        int  get_value(void);
        int  up(void);
        int  down(void);


        unsigned int  get_num_pin(void) const { return _num_pin; }
        int           set_direction(GPIO_Direction direction);
        int           get_direction(GPIO_Direction *direction);


        GPIO_Error get_errno(void) const { return _errno; }

        static const char* strerror(GPIO_Error error);


    private:


        GPIO_Error     _errno;
        int            _dev_fd;
        unsigned int   _num_pin;


        int gpio_open(unsigned int num_pin);
        int gpio_export(unsigned int num_pin);

};





#endif // LINUX_GPIO_H
