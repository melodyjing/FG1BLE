/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：transaction_processing_function.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains for transaction processing. 
************************************************************************/
#include "all_gpio_init.h"

u8 lxl_last_button_numbale = 0 ;      //当前按键的值

void key_count_ProcessOSALMsg( osal_event_hdr_t *pMsg )		//按键有效消息处理函数
{
    switch ( pMsg->event )
    {  
	    default:
        // do nothing
        break;
    }
}
//-------------------app 事件-------------------//
u16 APP_all_function(u8 task_id,u16 events)
{
	VOID task_id; // OSAL required parameter that isn't used in this function

	if ( events & SYS_EVENT_MSG )
	{
		uint8 *pMsg;
		if ( (pMsg = osal_msg_receive( APP_TaskID )) != NULL )
		{
			key_count_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );
			VOID osal_msg_deallocate( pMsg );							//清除消息内存
		}
		return (events ^ SYS_EVENT_MSG);
	}	
	

	static u32 get_ble_time = 0 ; 
	static u32 get_ble_connect_time = 0;
	static u32 ble_start_time = 0;
	//-------------------每次广播或和手机交互回调一次--------------------------//
	if(events&UI_BLE_TIMEOUT_TICK_1)	//连接成功后每和手机交互一次回调一次
	{            
     
		get_ble_connect_time = hal_ms_intv(get_ble_time);
		get_ble_time =  hal_systick()|1;
		ble_start_time = hal_systick()|1;   //起始时间
		//LOG("TIME= %d\n",get_ble_connect_time);
		return (events ^ UI_BLE_TIMEOUT_TICK_1);
	}
/*	if(events&UI_BLE_TIMEOUT_TICK_2)	//连接成功后每和手机交互一次回调一次
	{      
		get_ble_connect_time = hal_ms_intv(get_ble_time);
		get_ble_time =  hal_systick()|1;
		ble_start_time = hal_systick()|1;  //起始时间
		//LOG("TIME= %d\n",get_ble_connect_time);
		return (events ^ UI_BLE_TIMEOUT_TICK_2);
	} */ 
	if(events&UI_BLE_TIMEOUT_TICK_3)
	{
		static u8  temp_onoff_flag = 0;
		//LOG("TIME11= %d\n",hal_ms_intv(ble_start_time));
		if(hal_ms_intv(ble_start_time) - get_ble_connect_time >=7 )
		{
			if(device_all_data.device_onoff_flag)
			{
				temp_onoff_flag = 0;

			}
			else
			{
				if(!temp_onoff_flag)
				{
					temp_onoff_flag = 1;
		
				}
			}
			osal_start_timerEx(APP_TaskID, UI_BLE_TIMEOUT_TICK_3, 25);		 
		}
		if(device_all_data.device_onoff_flag)
		{
			osal_start_timerEx(APP_TaskID, UI_BLE_TIMEOUT_TICK_3, 8);
		}
		else
		{
			osal_stop_timerEx(APP_TaskID, UI_BLE_TIMEOUT_TICK_3);
		}
		return (events ^ UI_BLE_TIMEOUT_TICK_3);
	}
	//-------------------APP数据接收------------------------------------------//
    if(events&UI_100MS_EVENT_TICK)
	{
		u8 pValue[20] = {0} ;
				
//		uint8 push_data_app[20] = {0x00} ;	  //上传数据到app中
		for(u8 i=0; i<20; i++)
		{
			pValue[i] = device_all_data.dev_app_push_data[i] ;
		}
		// LOG("APP_DATA: %d -",device_all_data.dev_app_push_data[39]) ;
		uart_printf_hex(pValue,device_all_data.dev_app_push_data[39]) ;

		l_judge_app_data(pValue);
		//APP下方数据处理
		// lxl_SimpleProfile_SetParameter(1,5,push_data_app);
		return (events ^ UI_100MS_EVENT_TICK);
	}
//-------------------key 事件--------------------//	 
	if(KEY_EVENT_TICK_1 &events)//按键处理
	{   
		static u8 lxl_last_button_pressed = 0 ;      //当前按下只能进入一次 
		static u8 lxl_long_key_enable = 0 ;          //当前按下只能进入一次
		
		static u32 lxl_last_button_press_time = 0 ;  //所有按键抬起的时间
		static u32 lxl_last_button_release_time = 0 ;//所有按键按下的时间
		static u32 lxl_key_time_temp = 0 ;           //当前time以20ms加一作为时间单位
		
		
		lxl_key_time_temp = hal_systick()|1;
		if((device_all_data.key_dowm_tick)&&(device_all_data.device_onoff_flag))			//按下
		{
			if(lxl_last_button_numbale&&lxl_long_key_enable && clock_time_exceed_func(lxl_last_button_press_time,1500))	//按下1.5S
			{
				lxl_long_key_enable = 0 ;
				
				if(lxl_last_button_numbale&BIT(0))						//模式键
				{   
					if(device_all_data.R_DisPlay_Time==0)				//如果是息屏状态
					{
						if(device_all_data.device_onoff_flag)
						{
							device_all_data.R_DisPlay_Time =30;
						}
						else
						{
							device_all_data.R_DisPlay_Time=10;	
						}	
					}
					else
					{
						if(device_all_data.device_onoff_flag)				//在开机状态下
						{
							device_all_data.R_DisPlay_Time=30;
						}
						else												//关机状态下，
						{
							device_all_data.R_DisPlay_Time=10;				//显示10S
						}
						if(device_all_data.R_Work_Mold==2)
						{
							device_all_data.R_Work_Mold = 3;
							device_all_data.R_Work_Level = 1;
							R_Step_Cnt = 1;
						}
						else if(device_all_data.R_Work_Mold==3)
						{
							device_all_data.R_Work_Mold = 1;
							device_all_data.R_Work_Level = 1;	
						}
						else if(device_all_data.R_Work_Mold==1)
						{
							device_all_data.R_Work_Mold = 3;
							device_all_data.R_Work_Level = 1;
							R_Step_Cnt =1;
						}
					}
				}
				else if(lxl_last_button_numbale&BIT(1))				//长按开关键关机
				{       
					if(device_all_data.device_onoff_flag)
					{
						device_all_data.device_onoff_flag = 0;
						my_i2c_sim_burst_write(SW6206_Device,0x18,0x10);
						device_all_data.R_DisPlay_Time=0;	
						device_all_data.Music_Number=7;
						if(device_all_data.R_Work_Mold==1)
						{
							device_all_data.B_Cool_Delay =1;
							device_all_data.R_Cool_Delay =0;
						}
					}
				}
				if(device_all_data.R_DisPlay_Time)
				{
					if(device_all_data.R_Work_Mold==1)
					{
						 device_all_data.Music_Number =5;		
					}
					if(device_all_data.R_Work_Mold==2)
					{
						device_all_data.Music_Number =4;
					}
					if(device_all_data.R_Work_Mold==3)
					{
						device_all_data.Music_Number =6;
					}
				}
				lxl_last_button_numbale = 0 ;
			}
		}
		else
		{ 
			lxl_long_key_enable = 1 ;
		}

		if(device_all_data.key_dowm_tick)		//按下
		{
			if(!lxl_last_button_pressed&&clock_time_exceed_func(lxl_last_button_release_time,20))
			{
				lxl_last_button_pressed = 1 ;
				lxl_last_button_numbale = device_all_data.key_dowm_tick ;
				lxl_last_button_press_time = lxl_key_time_temp ;
			}
		}
		else 									//释放
		{
			if(lxl_last_button_pressed&&clock_time_exceed_func(lxl_last_button_press_time,20) )
			{
				lxl_last_button_release_time = lxl_key_time_temp;
				lxl_last_button_pressed = 0 ;
			}
		}
		if(lxl_last_button_numbale&&!device_all_data.key_dowm_tick&& clock_time_exceed_func(lxl_last_button_press_time,20))		//按下后释放处理
		{
			if((lxl_last_button_numbale&BIT(0))&&(device_all_data.device_onoff_flag))									//模式切换
			{   
				
				if(device_all_data.R_DisPlay_Time==0)						//如果是息屏状态
				{
					if(device_all_data.device_onoff_flag)
					{
						device_all_data.R_DisPlay_Time =30;
					}
					else
					{
						device_all_data.R_DisPlay_Time=10;	
					}
				}
				else
				{	
					if(device_all_data.device_onoff_flag)
					{
						device_all_data.R_DisPlay_Time =30;
					}
					else
					{
						device_all_data.R_DisPlay_Time=10;	
					}
					if(device_all_data.R_Work_Mold == 2)
					{
						device_all_data.R_Work_Mold = 1;					//制冷模式
						device_all_data.R_Work_Level= 1;	
					}
					else if(device_all_data.R_Work_Mold == 1)
					{
						device_all_data.R_Work_Mold = 2;					//风扇模式
						device_all_data.R_Work_Level= 1;		
					}
				}
			}
			else if(lxl_last_button_numbale&BIT(1))//开关
			{   		
				
				if(device_all_data.device_onoff_flag)
				{
					if(device_all_data.R_DisPlay_Time ==0)
					{
						device_all_data.R_DisPlay_Time =30;	
					}
					else
					{
						device_all_data.R_DisPlay_Time =30;	
						device_all_data.R_Work_Level++;
						
						if(device_all_data.R_Work_Level>3)
						{
							device_all_data.R_Work_Level=1;
							device_all_data.device_onoff_flag = 0;
							device_all_data.R_DisPlay_Time =0;
							
							device_all_data.Music_Number=7;
							R_Step_Cnt=1;
							osal_stop_timerEx(APP_TaskID, ADC_1_EVENT_TICK);
							osal_set_event(APP_TaskID,ADC_STOP_EVENT_TICK);
							osal_set_event(APP_TaskID,LED_EVENT_TICK);
							if(device_all_data.R_Work_Mold==1)
							{
								device_all_data.B_Cool_Delay =1;
								device_all_data.R_Cool_Delay =0;	
							}
							else
							{
								my_i2c_sim_burst_write(SW6206_Device,0x18,0x10);

								osal_set_event(APP_TaskID,EVENT_Sleep);
							}
						}
					}
				}
				else
				{
					device_all_data.device_onoff_flag =1;
					device_all_data.R_DisPlay_Time =30;
					my_i2c_sim_burst_write(SW6206_Device,0x18,0x00);
					HW_timer_100us();
					osal_start_reload_timer(APP_TaskID, LED_EVENT_TICK, 100);
					osal_set_event(APP_TaskID,ADC_1_EVENT_TICK);
					osal_start_reload_timer(APP_TaskID, HALFS_EVENT_TICK, 500);
//					LOG("3/n");
				}
			}
			device_all_data.key_dowm_time = lxl_key_time_temp ;    //计算下一次按键时间
			
//			LOG("KEY_UP_1111: %d\n",lxl_last_button_numbale) ;
			lxl_last_button_numbale = 0 ;
			if(device_all_data.R_DisPlay_Time)
			{
				if(device_all_data.R_Work_Mold==1)
				{
					 device_all_data.Music_Number =5;		
				}
				if(device_all_data.R_Work_Mold==2)
				{
					device_all_data.Music_Number =4;
				}
				if(device_all_data.R_Work_Mold==3)
				{
					device_all_data.Music_Number =6;
				}
			}
			if(device_all_data.key_dowm_tick )
			{
				osal_start_timerEx(APP_TaskID, KEY_EVENT_TICK_1, 40);
			}   
		}
//		LOG("events:0x%x\n",events);
		return (events ^ KEY_EVENT_TICK_1);
	}
//-------------------ADC 事件-------------------//	
	
	static u8 temp_count_adc = 0; 
	if(ADC_1_EVENT_TICK&events)		//开始
	{    
		if(device_all_data.get_adc_enable)
		{
			if(temp_count_adc<=4)
			{
				temp_count_adc++;
			}
		}
		else
		{
			temp_count_adc = 0;
		}
		if(temp_count_adc<=4)
		{
			hal_adc_stop() ;										//ADC停止
			hal_adc_init() ;										//ADC清除配置
			
			normal_adc_init() ;										//重新ADC初始化
			hal_adc_start() ;										//开启ADC
		}
		if((device_all_data.device_onoff_flag)==1)
		{
			osal_start_timerEx(APP_TaskID, ADC_1_EVENT_TICK, 50);
		}
		return (events ^ ADC_1_EVENT_TICK);
	}
	if(ADC_STOP_EVENT_TICK&events)	//停止
	{        
		hal_adc_stop() ;
	
		return (events ^ ADC_STOP_EVENT_TICK);
	}	
//-------------------100ms 事件-------------------//

	if(LED_EVENT_TICK&events)		//开始
	{   
		if(device_all_data.device_onoff_flag)
		{
			SW6206_DisCharge_Contorl();
		}
		else
		{
			SW6206_Charge_Contorl();
			if(hal_gpio_read(SW6206_IRQ)==1)
			{
				osal_set_event(APP_TaskID,EVENT_Sleep);	
			}
		}
		lxl_get_rtc_time_data_func();  										//获取当前时间
		l_judge_time();               										//判断定时开关
		command_send_app_data();      										//发送APP数据		
		NTC_Check();
		F_DisPlay();
		F_Work_Function();
//		LOG("events:0x%x\n",events);
		return (events ^ LED_EVENT_TICK);
	}
//-------------------500ms 事件-------------------//
	if(HALFS_EVENT_TICK & events)
	{
		B_500ms_Flag=!B_500ms_Flag;
		if(B_500ms_Flag)
		{
			Hot_Time_Contor();	
		}
		
//		LOG("events:0x%x\n",events);
		
		return (events^ HALFS_EVENT_TICK);
	}
	if(EVENT_Sleep & events)
	{
		int i,Sleep= 0;	
//		LOG("8\n");
//		LOG("events:0x%x\n",events);
//		LOG("onoff_flag:%d\n",device_all_data.device_onoff_flag);
//		LOG("R_DisPlay_Time:%d\n",device_all_data.R_DisPlay_Time);
//		LOG("B_Cool_Delay:%d\n",device_all_data.B_Cool_Delay);
		
		if(((device_all_data.device_onoff_flag)==0)&&((device_all_data.R_DisPlay_Time)==0)&&((device_all_data.B_Cool_Delay)==0))
		{
			hal_gpio_pin_init(KEY_SW_2, GPIO_INPUT);
			hal_gpio_pin_init(SW6206_IRQ, GPIO_INPUT);
			
			if(hal_pwrmgr_is_lock(MOD_USR1) == TRUE)
			{
				for(i = 0; i< 20; i++)
				{
					if((hal_gpio_read(KEY_SW_2)==1)&&(hal_gpio_read(SW6206_IRQ)==1))
					{
						Sleep= TRUE;		
						break;
					}
				}
				if(Sleep)
				{
//					LOG("4\n");
					osal_stop_timerEx(APP_TaskID, LED_EVENT_TICK);
					osal_stop_timerEx(APP_TaskID, KEY_EVENT_TICK_1);
					osal_stop_timerEx(APP_TaskID, HALFS_EVENT_TICK);
					osal_stop_timerEx(APP_TaskID, ADC_1_EVENT_TICK);
					osal_stop_timerEx(APP_TaskID, ADC_STOP_EVENT_TICK);
					uint8 initial_advertising_enable = FALSE;
					GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &initial_advertising_enable );
					hal_pwrmgr_unlock(MOD_USR1);	
				}
				else
				{
//					LOG("5\n");

					osal_set_event(APP_TaskID,EVENT_Sleep);
//					osal_start_timerEx(APP_TaskID, EVENT_Sleep, 5000);
				}	
			}

		}
		return (events^ EVENT_Sleep);
	}
	
	return 0 ;
}






	
	
	
	



