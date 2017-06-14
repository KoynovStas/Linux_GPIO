/*
 * linux_gpio.h
 *
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2015, Koynov Stas - skojnov@yandex.ru
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

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
