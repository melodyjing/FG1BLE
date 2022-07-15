/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：peripheral_handler.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains for peripheral handler. 
************************************************************************/
#include "all_gpio_init.h"

//=================中断函数======================//
	bool B_Wake_Up;

//IO中断响应
void pin_event_handler_key(GPIO_Pin_e pin,IO_Wakeup_Pol_e type)
{
	switch(pin)
	{
		case KEY_SW_1:
			if(type == NEGEDGE)
			{
				device_all_data.key_dowm_tick |= BIT(0);
			}
			else
			{
				device_all_data.key_dowm_tick &= ~BIT(0);
			}
			osal_start_timerEx(APP_TaskID, KEY_EVENT_TICK_1, 10);
			break;
			
		case KEY_SW_2:
			if(type == NEGEDGE)
			{
				device_all_data.key_dowm_tick |= BIT(1);
				
/*				if(device_all_data.device_onoff_flag == 0)
				{
					lxl_last_button_numbale = device_all_data.key_dowm_tick ;
					hal_pwrmgr_lock(MOD_USR1);
					IO_Init();
				}*/
			}
			else 
			{
				device_all_data.key_dowm_tick &= ~BIT(1) ;
				
			}
			osal_start_timerEx(APP_TaskID, KEY_EVENT_TICK_1, 10);
			break;	
		default:
			break;
	} 
}

__ATTR_SECTION_SRAM__ void gpio_sleep_handle()
{
    hal_gpio_pin_init(KEY_SW_2, GPIO_INPUT);
    hal_gpio_fmux(KEY_SW_2, Bit_DISABLE);
    extern void hal_gpioin_set_flag(gpio_pin_e pin);  // in gpio.c file.
    hal_gpioin_set_flag(KEY_SW_2);
    hal_gpio_wakeup_set(KEY_SW_2, NEGEDGE); //  NEGEDGE
//	LOG("1\n");
    hal_gpio_pin_init(SW6206_IRQ, GPIO_INPUT);
	hal_gpio_fmux(SW6206_IRQ, Bit_DISABLE);
    extern void hal_gpioin_set_flag(gpio_pin_e pin);  // in gpio.c file.
	hal_gpioin_set_flag(SW6206_IRQ);
    hal_gpio_wakeup_set(SW6206_IRQ, NEGEDGE); //  NEGEDGE		
}

__ATTR_SECTION_SRAM__ void gpio_wakeup_handle()
{
    int i,flag = 0;

//    uint8_t ret = 0;
    hal_gpio_pin_init(KEY_SW_2, GPIO_INPUT);
//    hal_gpioin_disable(KEY_SW_2);
    hal_gpio_pin_init(SW6206_IRQ, GPIO_INPUT);
//    hal_gpioin_disable(SW6206_IRQ);	

    if(hal_pwrmgr_is_lock(MOD_USR1) == FALSE)
    {
        for(i = 0; i< 20; i++)
        {
            if(hal_gpio_read(KEY_SW_2)==0)
            {
                flag = TRUE;
				device_all_data.key_dowm_tick |= BIT(1);
				lxl_last_button_numbale = device_all_data.key_dowm_tick ;
				IO_Init();
				osal_start_reload_timer(APP_TaskID, KEY_EVENT_TICK_1, 20);		
                break;
            }
			if(hal_gpio_read(SW6206_IRQ)==0)
			{
				flag = TRUE;
//				IO_Init();
				HW_timer_100us();
				osal_start_reload_timer(APP_TaskID, LED_EVENT_TICK, 100);
				osal_start_reload_timer(APP_TaskID, HALFS_EVENT_TICK, 500);
				break;
			}			
        }
        if( flag )
        {
            for(i = 0; i< 40; i++)
            {
				if(device_all_data.device_onoff_flag==0)
				{
					if((hal_gpio_read(KEY_SW_2)==0)||(hal_gpio_read(SW6206_IRQ)==0))	//&&(hal_gpio_read(SW6206_IRQ)==0)
					{
						uint8 initial_advertising_enable = TRUE;
						GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &initial_advertising_enable );
						hal_pwrmgr_lock(MOD_USR1);
						break;
					}
				}
            }
        }
//		osal_start_timerEx(APP_TaskID, EVENT_Sleep, 5000);
//		hal_pwrmgr_lock(MOD_USR1);
//		LOG("2\n");
		
		
    }
}


