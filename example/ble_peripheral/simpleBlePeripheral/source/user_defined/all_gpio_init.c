/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：all_gpio_init.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains for init all gpio. 
************************************************************************/
#include "all_gpio_init.h"

u8 APP_TaskID;						//个人ui事件的id

struct_device_all device_all_data = 
{
	.device_onoff_flag      = 0,        //设备开关标志位
	.R_Work_Mold			= 0,					//工作模式
	.R_Work_Level			= 0,				//工作档位
	.R_DisPlay_Time			= 0,		//按键显示时间
	.Music_Number			= 0,
	.NTC1_Value				= 0,
	.NTC2_Value				= 0,
	
	.device_time_on         ={0},		//当前定时开设置状态  0:当前定时开关  1:工作星期  2:小时 3:分钟
	.device_time_off        ={0},		//当前定时关设置状态
	.get_adc_enable         = 0,		//避开DMA中断

	.dev_app_push_data      ={0},		//app下发数据缓存
	.key_dowm_tick          = 0,
	.key_dowm_time          = 0,
	.R_Cool_Delay			=0,
	
	.B_Cool_Delay			=0,
	.B_NTC_Err				=0,
	.B_NTC_CoolErr			=0,
	.B_NTC_HotErr			=0,
};
void IO_Init(void)
{
	//-------------------------------------------------//
	hal_gpio_pin_init(KEY_SW_2,IE) ;                  //设置为输入
	hal_gpio_pull_set(KEY_SW_2,STRONG_PULL_UP) ;      //设置当前io的上下拉
	hal_gpioin_register(KEY_SW_2, pin_event_handler_key, pin_event_handler_key);

	hal_gpio_pin_init(KEY_SW_1,IE) ;                  //设置为输入
	hal_gpio_pull_set(KEY_SW_1,STRONG_PULL_UP) ;      //设置当前io的上下拉
	hal_gpioin_register(KEY_SW_1, pin_event_handler_key, pin_event_handler_key);
	
  //-----------------------------------------------//
	IIC_init();
	hal_gpio_pin_init(SW6206_IRQ,IE) ;               
	hal_gpio_pull_set(SW6206_IRQ,STRONG_PULL_UP) ;      //设置当前io的上下拉
	hal_gpioin_register(SW6206_IRQ, pin_event_handler_key, pin_event_handler_key);
	
	hal_gpio_pin_init(LED_GPIO_A,OEN) ;               
	hal_gpio_write(LED_GPIO_A,0);
		
	hal_gpio_pin_init(LED_GPIO_B,OEN) ;               
	hal_gpio_write(LED_GPIO_B,0);	
				        
	hal_gpio_pin_init(LED_GPIO_C,OEN) ;               
	hal_gpio_write(LED_GPIO_C,0);	
				        
	hal_gpio_pin_init(LED_GPIO_D,OEN) ;               
	hal_gpio_write(LED_GPIO_D,0);	
	  			        
	hal_gpio_pin_init(LED_GPIO_E,OEN) ;               
	hal_gpio_write(LED_GPIO_E,0);	
	  			        		
	
	hal_gpio_pin_init(Cool_Contorl_A,OEN) ;               
	hal_gpio_write(Cool_Contorl_A,0);	
	  			        
	hal_gpio_pin_init(Cool_Contorl_B,OEN) ;               
	hal_gpio_write(Cool_Contorl_B,0);	
       
	hal_gpio_pin_init(Fan_GPIO,OEN) ;               
	hal_gpio_write(Fan_GPIO,0);	
	     
	hal_gpio_pin_init(Speech_Contorl,OEN) ;               
	hal_gpio_write(Speech_Contorl,0);
	
	//----------------ADC控制----------------------------//  
	hal_gpio_cfg_analog_io(NTC1_CHANNEL, Bit_ENABLE);		//NTC1 Channel
	hal_gpio_cfg_analog_io(NTC2_CHANNEL, Bit_ENABLE);		//NTC2 Channel	
	
	
}



void gpio_all_init_func(void)
{

	IO_Init();
	device_all_data.R_Work_Mold = 1;
	device_all_data.R_Work_Level = 1;

	lxl_set_rtc_clock_data_func(1626659910);					//设置RTC时间
    hal_pwrmgr_register(MOD_USR1, gpio_sleep_handle, gpio_wakeup_handle);
    hal_pwrmgr_lock(MOD_USR1);
	uint8 initial_advertising_enable = FALSE;
	GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &initial_advertising_enable );
	hal_pwrmgr_unlock(MOD_USR1);
}

void APP_init_task_func(uint8 task_id)
{
	APP_TaskID = task_id;	
	gpio_all_init_func();	
}


void all_gpio_input(void)
{
	hal_gpio_pin_init(LED_GPIO_A,IE) ;
	hal_gpio_pull_set(LED_GPIO_A,GPIO_FLOATING) ;
	hal_gpio_pin_init(LED_GPIO_B,IE) ;
	hal_gpio_pull_set(LED_GPIO_B,GPIO_FLOATING) ;
	hal_gpio_pin_init(LED_GPIO_C,IE) ;
	hal_gpio_pull_set(LED_GPIO_C,GPIO_FLOATING) ;
	hal_gpio_pin_init(LED_GPIO_D,IE) ;
	hal_gpio_pull_set(LED_GPIO_D,GPIO_FLOATING) ;
	hal_gpio_pin_init(LED_GPIO_E,IE) ;
	hal_gpio_pull_set(LED_GPIO_E,GPIO_FLOATING) ;
}

void GPIO_A_High(void)
{
	hal_gpio_pin_init(LED_GPIO_A,OEN) ;               
	hal_gpio_write(LED_GPIO_A,1);	
}

void GPIO_A_Low(void)
{
	hal_gpio_pin_init(LED_GPIO_A,OEN) ;               
	hal_gpio_write(LED_GPIO_A,0);	
}

void GPIO_B_High(void)
{
	hal_gpio_pin_init(LED_GPIO_B,OEN) ;               
	hal_gpio_write(LED_GPIO_B,1);	
}

void GPIO_B_Low(void)
{
	hal_gpio_pin_init(LED_GPIO_B,OEN) ;               
	hal_gpio_write(LED_GPIO_B,0);	
}

void GPIO_C_High(void)
{
	hal_gpio_pin_init(LED_GPIO_C,OEN) ;               
	hal_gpio_write(LED_GPIO_C,1);	
}

void GPIO_C_Low(void)
{
	hal_gpio_pin_init(LED_GPIO_C,OEN) ;               
	hal_gpio_write(LED_GPIO_C,0);	
}

void GPIO_D_High(void)
{
	hal_gpio_pin_init(LED_GPIO_D,OEN) ;               
	hal_gpio_write(LED_GPIO_D,1);	
}

void GPIO_D_Low(void)
{
	hal_gpio_pin_init(LED_GPIO_D,OEN) ;               
	hal_gpio_write(LED_GPIO_D,0);	
}

void GPIO_E_High(void)
{
	hal_gpio_pin_init(LED_GPIO_E,OEN) ;               
	hal_gpio_write(LED_GPIO_E,1);	
}

void GPIO_E_Low(void)
{
	hal_gpio_pin_init(LED_GPIO_E,OEN) ;               
	hal_gpio_write(LED_GPIO_E,0);	
}

void Hot_Open(void)
{
	hal_gpio_write(Cool_Contorl_A,0);	
	hal_gpio_write(Cool_Contorl_B,1);	
}

void Cool_Open(void)
{
	hal_gpio_write(Cool_Contorl_B,0);	
	hal_gpio_write(Cool_Contorl_A,1);
}

void Hot_Cool_Close(void)
{
	hal_gpio_write(Cool_Contorl_B,0);	
	hal_gpio_write(Cool_Contorl_A,0);
}

//---------------------BLE -> APP-------------------------//
void lxl_SimpleProfile_SetParameter(uint8 param, uint8 len, void *value )
{
	//SimpleProfile_SetParameter(param,len,value) ;
	simpleProfile_Notify(param,len,value) ;
	LOG("APP_DATA: %d  ",len) ;
	uart_printf_hex(value,len) ;
}

void uart_printf_hex (const uint8 *data, uint16 len)
{
    uint16 i;

    for (i = 0; i < len - 1; i++)
    {
        LOG("%x ",data[i]);
    }
    LOG("%x\n",data[i]);
}

u32 clock_time_exceed_func(u32 ref, u32 span_ms)
{
	u32 deltTick  = 0 ;
	deltTick = hal_ms_intv(ref) ;
	if(deltTick>span_ms)
	{
		return 1 ;
	}
	else 
	{
		return 0 ;
	}	
}
