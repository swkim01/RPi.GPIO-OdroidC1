/*
Copyright (c) 2015 Dong-Seok Lee, Seong-Woo Kim

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "c_gpio.h"
#include "common.h"
#include "hard_pwm.h"

#define PWM_DIR "/sys/devices/platform/pwm-ctrl/"
#define PWM_NO(gpio) (108-( *(pinToGpioOdroidC + gpio)))

int hwpwm_check_valid(unsigned int gpio){
    char filename[50];
    unsigned pwm_no=PWM_NO(gpio);
    if(pwm_no!=0 && pwm_no!=1)
       return 1;
    if(access(PWM_DIR,F_OK)!=0)
        return 2; 
    snprintf(filename,sizeof(filename),PWM_DIR "enable%u",pwm_no);
    if(access(filename,F_OK)!=0)
        return 3;
    return 0;
}

void hwpwm_set_duty_cycle(unsigned int gpio, float dutycycle)
{
    char filename[50];
    char str_pwm[5];
    unsigned int pwm_cycle;
    unsigned int pwm_no=PWM_NO(gpio);
    if(dutycycle<0) pwm_cycle=0;
    else if(dutycycle>100) pwm_cycle=1023;
    else
        pwm_cycle=1023*(dutycycle/100.0);
    snprintf(filename,sizeof(filename),PWM_DIR "duty%u",pwm_no);
    int fd;
    if((fd=open(filename,O_WRONLY))<0)
        return;
    int len=snprintf(str_pwm,sizeof(str_pwm),"%u",pwm_cycle);
    write(fd,str_pwm,len);
    close(fd);
}

void hwpwm_set_frequency(unsigned int gpio, float freq)
{
    char filename[50];
    char str_pwm[4];
    unsigned int pwm_no=PWM_NO(gpio); 
    if (freq <= 0.0) // to avoid divide by zero
    {
        // btc fixme - error
        return;
    }
    snprintf(filename,sizeof(filename),PWM_DIR "freq%u",pwm_no);
    int fd;
    if((fd=open(filename,O_WRONLY))<0)
        return;
    int len=snprintf(str_pwm,sizeof(str_pwm),"%d",(int)freq);
    write(fd,str_pwm,len);
    close(fd);
}

void hwpwm_start(unsigned int gpio)
{
    char filename[50];
    char str_pwm[4];
    unsigned int pwm_no=PWM_NO(gpio); 
    snprintf(filename,sizeof(filename),PWM_DIR "enable%u",pwm_no);
    int fd;
    if((fd=open(filename,O_WRONLY))<0)
        return;
    int len=snprintf(str_pwm,sizeof(str_pwm),"1");
    write(fd,str_pwm,len);
    close(fd);
    return;
}

void hwpwm_stop(unsigned int gpio)
{    
    char filename[50];
    char str_pwm[4];
    unsigned int pwm_no=PWM_NO(gpio); 
    snprintf(filename,sizeof(filename),PWM_DIR "enable%u",pwm_no);
    int fd;
    if((fd=open(filename,O_WRONLY))<0)
        return;
    int len=snprintf(str_pwm,sizeof(str_pwm),"0");
    write(fd,str_pwm,len);
    close(fd);
    return;
}
