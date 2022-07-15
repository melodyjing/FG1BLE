/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：set_gpio_pwm.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains for pwm. 
************************************************************************/
#include "all_gpio_init.h"


void Fan_pwm_timeout_handle(void)   //init
{
//	hal_gpio_write(Fan_GPIO, 0);
	hal_pwm_close_channel(PWM_CH0);
	hal_pwm_destroy(PWM_CH0);
	hal_pwm_stop();
}

void Fan_on(void)
{
	hal_pwm_init(PWM_CH0, PWM_CLK_DIV_4, PWM_CNT_UP, PWM_POLARITY_FALLING);   //16M/8 = 2M
	hal_pwm_open_channel(PWM_CH0, Fan_GPIO);
}

//-------------占空比输出函数-------------//
void Fan_pwm_ctrl(u8 Fan_PWM)
{
	if(Fan_PWM ==0)
	{
		hal_gpio_write(Fan_GPIO, 0);
	}
	else
	{
		Fan_on() ;
		hal_pwm_set_count_val(PWM_CH0, Fan_PWM, SET_PWM_CNTTOP_VALUE);
		hal_pwm_start();
	}  
}

//------------------PWM关断-------------------//
void Fan_off(void)
{
	hal_pwm_close_channel(PWM_CH0);
	hal_pwm_destroy(PWM_CH0);
	hal_pwm_stop();
	hal_gpio_write(Fan_GPIO, 0);
}
