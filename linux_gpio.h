#ifndef LINUX_GPIO_H
#define LINUX_GPIO_H





class Linux_GPIO
{    

    public:


        enum GPIO_Error{

            ERROR_DEV_NOT_OPEN,
            ERROR_CANT_OPEN_DEV,
            ERROR_CANT_OPEN_EXPORT,
            ERROR_CANT_OPEN_DIRECTION,
            ERROR_CANT_READ,
            ERROR_CANT_WRITE
        };


        enum GPIO_Direction{

            GPIO_IN,
            GPIO_OUT
        };


         Linux_GPIO();
        ~Linux_GPIO();


        void dev_close(void);

        unsigned int get_num_pin(void){ return _num_pin; }

        GPIO_Error get_errno(void){ return _errno; }

        static const char* strerror(GPIO_Error errno);


    private:


        GPIO_Error   _errno;
        int          _dev_fd;
        unsigned int _num_pin;
};





#endif // LINUX_GPIO_H
