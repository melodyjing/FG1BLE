/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：other_function.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains for other function. 
************************************************************************/
#include "all_gpio_init.h"
u8 Led_Tab[17] ={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x76};
u8  led_scan=0;
u8 LED_NUM[3];
u8 R_Step_Cnt=0;
u16 R_Time_Cnt =0;
u16 R_Set_Value=0;
bool B_Start_Time =0;
//bool B_SanPID;
bool B_500ms_Flag=0;							//控制温度
bool B_Out_H =0;

bool B_OutPut=0;


void Led_Driver(void)
{
	if(++led_scan>=17) led_scan= 0;
	all_gpio_input();
	switch(led_scan)
	{
		case 0:
		if(LED_NUM[0]&0x01)
		{
			GPIO_E_High();
			GPIO_D_Low();
		}
		break;
		
		case 1:
		if(LED_NUM[0]&0x02)
		{
			GPIO_D_High();
			GPIO_E_Low();	
		}
		break;
		
		case 2:
		if(LED_NUM[0]&0x04)
		{
			GPIO_E_High();
			GPIO_C_Low();			
		}
		break;
		
		case 3:			
		if(LED_NUM[0]&0x08)
		{
			GPIO_C_High();
			GPIO_E_Low();			
		}
		break;
		
		case 4:		
		if(LED_NUM[0]&0x10)
		{
			GPIO_E_High();
			GPIO_B_Low();				
		}
		break;
		
		case 5:	
		if(LED_NUM[0]&0x20)
		{
			GPIO_B_High();
			GPIO_E_Low();				
		}
		break;
		
		case 6:				
		if(LED_NUM[0]&0x40)
		{
			GPIO_A_High();
			GPIO_E_Low();				
		}
		break;
		
		case 7:				
		if(LED_NUM[0]&0x80)
		{
			GPIO_D_High();
			GPIO_A_Low();				
		}
		break;
		
		case 8:				
		if(LED_NUM[1]&0x01)
		{
			GPIO_D_High();
			GPIO_C_Low();			
		}
		break;
		
		case 9:				
		if(LED_NUM[1]&0x02)
		{
			GPIO_C_High();
			GPIO_D_Low();			
		}
		break;
		
		case 10:				
		if(LED_NUM[1]&0x04)
		{
			GPIO_B_High();
			GPIO_C_Low();
		}
		break;
		
		case 11:				
		if(LED_NUM[1]&0x08)
		{
			GPIO_B_High();
			GPIO_D_Low();	
		}
		break;
		
		case 12:				
		if(LED_NUM[1]&0x10)
		{
			GPIO_A_High();
			GPIO_D_Low();	
		}
		break;
		
		case 13:	
		if(LED_NUM[1]&0x20)
		{
			GPIO_A_High();
			GPIO_C_Low();	
		}
		break;
		
		case 14:				
		if(LED_NUM[1]&0x40)
		{
			GPIO_A_High();
			GPIO_B_Low();			
		}
		break;
		
		case 15:				
		if(LED_NUM[2]&0x02)
		{
			GPIO_C_High();
			GPIO_B_Low();
		}
		break;
		
		case 16:				
		if(LED_NUM[2]&0x04)
		{
			GPIO_D_High();
			GPIO_B_Low();
		}
		break;
		
		default : break;	
	}

}

void HW_timer_callback(uint8 evt)       //时间累加
{ 
	if(evt==HAL_EVT_TIMER_5) 
	{   
		Led_Driver();
		if(device_all_data.Music_Number)
		{
			if(!B_Out_H)
			{
				hal_gpio_write(Speech_Contorl,1);
				B_Out_H = 1;		
			}
			else
			{
				hal_gpio_write(Speech_Contorl,0);
				device_all_data.Music_Number--;
				B_Out_H =0;		
			}	
		}
		else
		{
			hal_gpio_write(Speech_Contorl,0);
			B_Out_H =0;			
		}
	}
}

void HW_time_100us_stop(void)      //关断定时器
{
	hal_timer_stop(AP_TIMER_ID_5) ;
}

void HW_timer_100us(void)     //定时器设置
{ 
	hal_timer_init(HW_timer_callback); 
	hal_timer_set(AP_TIMER_ID_5,100); 
} 


struct_PID_ALL PID_Value =
{
	.LiEKValue ={0},		//差值保存，给定和反馈的差值
	.uEKFlag ={0},			//符号，1则对应的为负数，0则对应的为正数
	.uKP_Coe =0,			//比例系数
	.uKI_Coe =0,			//积分常数
	.uKD_Coe =0,			//微分常数
	.iPriValue =0,			//上一时刻值
	.iSetValue =0,			//设定值
	.iCurValue =0,			//实际值
};

void PID_Operation(void)
{
	u32 temp[3]={0};	//中间临时变量
	u32 PostSum=0;		//正数之和
	u32 NegSum=0;		//负数之和
	
	if(PID_Value.iSetValue > PID_Value.iCurValue)
	{
		if(PID_Value.iSetValue-PID_Value.iCurValue>10)						//偏差值大于10？
		{
			PID_Value.iPriValue=100;										//全速输出
		}
		else
		{
			temp[0] = PID_Value.iSetValue-PID_Value.iCurValue;
			PID_Value.uEKFlag[1] = 0;
			PID_Value.LiEKValue[2] =PID_Value.LiEKValue[1];
			PID_Value.LiEKValue[1] =PID_Value.LiEKValue[0];
			PID_Value.LiEKValue[0] =temp[0];
			
			if(PID_Value.LiEKValue[0]>PID_Value.LiEKValue[1])
			{
				temp[0] = PID_Value.LiEKValue[0]-PID_Value.LiEKValue[1];
				PID_Value.uEKFlag[0] =0;
			}
			else
			{
				temp[0] = PID_Value.LiEKValue[1]-PID_Value.LiEKValue[0];
				PID_Value.uEKFlag[0] = 1;
			}
			temp[2] = 2*PID_Value.LiEKValue[1];								//2E(K-1)
			if((PID_Value.LiEKValue[0]+PID_Value.LiEKValue[2])>temp[2])  	//E(K-2)+E(K)>2E(k-1)?
			{
				temp[2] =(PID_Value.LiEKValue[0]+PID_Value.LiEKValue[2])-temp[2];
				PID_Value.uEKFlag[0] = 0;
			}
			else
			{
				temp[2] = temp[2]-(PID_Value.LiEKValue[0]+PID_Value.LiEKValue[2]);
				PID_Value.uEKFlag[1] = 1;
			}
			temp[0] = PID_Value.uKP_Coe*temp[0];						//KP*(E(k)-E(k-1))
			temp[1] = PID_Value.uKI_Coe*PID_Value.LiEKValue[1];			//KI*E(k)
			temp[2] = PID_Value.uKD_Coe*temp[2];						//KD*(E(k-2)+E(k)-2E(k-1))
			
			if(PID_Value.uEKFlag[0]==0)
			{
				PostSum += temp[0];
			}
			else
			{
				NegSum += temp[0];
			}
			if(PID_Value.uEKFlag[1]==0)
			{
				PostSum += temp[1];
			}
			else
			{
				NegSum += temp[1];
			}
			if(PID_Value.uEKFlag[2]==0)
			{
				PostSum += temp[2];
			}
			else
			{
				NegSum += temp[2];
			}	
			PostSum += PID_Value.iPriValue;
			if(PostSum>NegSum)							//判断控制量是否为负数
			{
				temp[0]= PostSum-NegSum;
				if(temp[0]<100)
				{
					PID_Value.iPriValue=temp[0];
				}
				else
				{
					PID_Value.iPriValue=100;	
				}
			}
			else
			{
				PID_Value.iPriValue=0;
			}
		}
	}
	else
	{
		PID_Value.iPriValue=0;
	}
}

void PID_Output(void)
{
	if(device_all_data.NTC1_Value>R_Set_Value)
	{
		Hot_Open();
	}
	else
	{
		Hot_Cool_Close();
	}
	
	
/*	u16 itemp;
	u8  uCounter;
	itemp = PID_Value.iPriValue;
	if(itemp ==0)
	{
		Hot_Open();				//打开发热
	}
	else
	{
		Hot_Cool_Close();		//关闭发热
	}
	if(B_SanPID)
	{
		B_SanPID= 0;
		if(itemp) itemp--;
		uCounter++;
		if(uCounter>=100)
		{
			uCounter =0;
			PID_Operation();			
		}	
	}*/
}
/***************************************************************
*
*
*1S任务。
**************************************************************/
void Hot_Time_Contor(void)
{
	u16 R_Time_Count;
	if((device_all_data.device_onoff_flag)&&(device_all_data.R_Work_Mold==3))
	{
		if(B_Start_Time)
		{
			if(++R_Time_Count>= R_Time_Cnt)
			{
				R_Time_Count= 0;
				R_Step_Cnt++;
				if(R_Step_Cnt>3)
				{
					R_Step_Cnt= 2;
				}
				B_Start_Time=0;
			}
		}
	}
	if(device_all_data.R_DisPlay_Time)							//显示时间
	{
		device_all_data.R_DisPlay_Time--;	
	}
	if(device_all_data.B_Cool_Delay)
	{
		if(++device_all_data.R_Cool_Delay>=2*60)
		{
			device_all_data.R_Cool_Delay =0;
			device_all_data.B_Cool_Delay = 0;
			my_i2c_sim_burst_write(SW6206_Device,0x18,0x10);
			osal_set_event(APP_TaskID,EVENT_Sleep);
		}
	}
}
/***************************************************************
*
*
*加热到43°维持1min,温度降到40度，维持10分钟，温度上升到43°，维持5min
**************************************************************/
void Hot_Mold_Level_One(void)
{
	if(R_Step_Cnt==1)
	{
		R_Set_Value = 328;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 1*60;
		}
	}
	if(R_Step_Cnt==2)
	{
		R_Set_Value = 355;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 10*60;
		}
	}	
	if(R_Step_Cnt==3)
	{
		R_Set_Value = 328;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 5*60;
		}
	}	
	PID_Output();					//控制温度
}
/***************************************************************
*
*
*加热到47°维持1min,温度降到45度，维持10分钟，温度降到40°，维持5min
**************************************************************/
void Hot_Mold_Level_Two(void)
{
	if(R_Step_Cnt==1)
	{
		R_Set_Value = 294;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 1*60;
		}
	}
	if(R_Step_Cnt==2)
	{
		R_Set_Value = 311;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 10*60;
		}
	}	
	if(R_Step_Cnt==3)
	{
		R_Set_Value = 355;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 5*60;
		}
	}		
	PID_Output();					//控制温度
}
/***************************************************************
*
*
*加热到53°维持1min,温度降到50度，维持10分钟，温度降到46°，维持5min
**************************************************************/
void Hot_Mold_Level_Thr(void)
{
	if(R_Step_Cnt==1)
	{
		R_Set_Value = 248;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 1*60;
		}
	}
	if(R_Step_Cnt==2)
	{
		R_Set_Value = 270;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 10*60;
		}
	}	
	if(R_Step_Cnt==3)
	{
		R_Set_Value = 302;
		if(device_all_data.NTC2_Value<=R_Set_Value)
		{
			B_Start_Time =1;
			R_Time_Cnt = 5*60;
		}
	}		
	PID_Output();					//控制温度	
}
/***************************************************************
*
*
*工作模式
**************************************************************/
void F_Work_Function(void)
{
	if(device_all_data.device_onoff_flag)
	{
		if(device_all_data.R_Work_Mold==1)
		{
			if(device_all_data.R_Work_Level ==1)
			{
				Fan_pwm_ctrl(80);
				Cool_Open();
			}
			if(device_all_data.R_Work_Level ==2)
			{
				Fan_pwm_ctrl(128);	
				Cool_Open();				
			}
			if(device_all_data.R_Work_Level ==3)
			{
				Fan_pwm_ctrl(160);
				Cool_Open();	
			}			
		}
		if(device_all_data.R_Work_Mold==2)
		{
			if(device_all_data.R_Work_Level ==1)
			{
				Fan_pwm_ctrl(80);
			}
			if(device_all_data.R_Work_Level ==2)
			{
				Fan_pwm_ctrl(128);	
			}
			if(device_all_data.R_Work_Level ==3)
			{
				Fan_pwm_ctrl(160);					
			}	
			Hot_Cool_Close();	
		}
		if(device_all_data.R_Work_Mold==3)
		{
			if(device_all_data.R_Work_Level ==1)
			{
				Hot_Mold_Level_One();	
			}
			if(device_all_data.R_Work_Level ==2)
			{
				Hot_Mold_Level_Two();
			}
			if(device_all_data.R_Work_Level ==3)
			{
				Hot_Mold_Level_Thr();
			}
			Fan_pwm_ctrl(0);
		}
	}
	else
	{
		Fan_pwm_ctrl(0);
		Fan_off();
		if(device_all_data.B_Cool_Delay)			//制冷片50%
		{
			if(B_OutPut)
			{
				B_OutPut=0;
				Cool_Open();
			}
			else
			{
				B_OutPut=1;
				Hot_Cool_Close();	
			}	
		}
		else
		{
			Hot_Cool_Close();	
		}
	}	
}


void NTC_Check(void)
{
	u16 D_Value;
	if((device_all_data.NTC1_Value)<20||(device_all_data.NTC2_Value<20)||(device_all_data.NTC2_Value>1000)||(device_all_data.NTC2_Value>1000))
	{
		device_all_data.B_NTC_Err=1;
	}
	if(device_all_data.device_onoff_flag)
	{
		if(device_all_data.R_Work_Mold ==1)
		{
			D_Value = device_all_data.NTC1_Value>device_all_data.NTC2_Value?device_all_data.NTC1_Value-device_all_data.NTC2_Value:device_all_data.NTC2_Value-device_all_data.NTC1_Value;
			if(D_Value>20)
			{
				device_all_data.B_NTC_CoolErr=1;
			}
		}
		if(device_all_data.R_Work_Mold ==3)
		{
			D_Value = device_all_data.NTC1_Value>device_all_data.NTC2_Value?device_all_data.NTC1_Value-device_all_data.NTC2_Value:device_all_data.NTC2_Value-device_all_data.NTC1_Value;
			if(D_Value>20)
			{
				device_all_data.B_NTC_HotErr=1;
			}
		}	
	}
}

void F_DisPlay(void)
{
	if(device_all_data.device_onoff_flag)
	{
		
		if((device_all_data.R_Work_Mold==1)&&(device_all_data.R_DisPlay_Time>26))
		{
			if(device_all_data.R_Work_Level ==1)				//显示C1
			{
				LED_NUM[0] =Led_Tab[1];
				LED_NUM[1] =Led_Tab[0x0C];	
				LED_NUM[2] =0;					
			}
			if(device_all_data.R_Work_Level ==2)				//显示C2
			{
				LED_NUM[0] =Led_Tab[2];
				LED_NUM[1] =Led_Tab[0x0C];		
				LED_NUM[2] =0;	
			}
			if(device_all_data.R_Work_Level ==3)				//显示C3
			{
				LED_NUM[0] =Led_Tab[3];
				LED_NUM[1] =Led_Tab[0x0C];		
				LED_NUM[2] =0;					
			}			
		}
		else if((device_all_data.R_Work_Mold==2)&&(device_all_data.R_DisPlay_Time>26))
		{
			if(device_all_data.R_Work_Level ==1)				//显示F1
			{
				LED_NUM[0] =Led_Tab[1];
				LED_NUM[1] =Led_Tab[0x0F];		
				LED_NUM[2] =0;	
			}
			if(device_all_data.R_Work_Level ==2)				//显示F2
			{
				LED_NUM[0] =Led_Tab[2];
				LED_NUM[1] =Led_Tab[0x0F];		
				LED_NUM[2] =0;	
			}
			if(device_all_data.R_Work_Level ==3)				//显示F3
			{
				LED_NUM[0] =Led_Tab[3];
				LED_NUM[1] =Led_Tab[0x0F];		
				LED_NUM[2] =0;
			}
		}
		else if((device_all_data.R_Work_Mold==3)&&(device_all_data.R_DisPlay_Time>26))
		{
			if(device_all_data.R_Work_Level ==1)				//显示H1
			{
				LED_NUM[0] =Led_Tab[1];
				LED_NUM[1] =Led_Tab[0x10];		
				LED_NUM[2] =0;				
			}
			if(device_all_data.R_Work_Level ==2)				//显示H2
			{
				LED_NUM[0] =Led_Tab[2];
				LED_NUM[1] =Led_Tab[0x10];		
				LED_NUM[2] =0;						
			}
			if(device_all_data.R_Work_Level ==3)				//显示H3
			{
				LED_NUM[0] =Led_Tab[3];
				LED_NUM[1] =Led_Tab[0x10];		
				LED_NUM[2] =0;			
			}
		}
		else if(device_all_data.R_DisPlay_Time)
		{
			if(SW6206_all_Data.BatTure_Data/100)
			{
				LED_NUM[2] =Led_Tab[SW6206_all_Data.BatTure_Data/100];
			}
			else
			{
				LED_NUM[2] = 0;
			}
			LED_NUM[1] =Led_Tab[(SW6206_all_Data.BatTure_Data%100/10)];
			LED_NUM[0] =Led_Tab[SW6206_all_Data.BatTure_Data%10]|0x80;								//0x80为%
			if((SW6206_all_Data.IRQ_Data&0x04)&&(B_500ms_Flag))
			{
				LED_NUM[0] &=~0x7f;
			}	
		}
		else
		{
			LED_NUM[0] =0;
			LED_NUM[1] =0;		
			LED_NUM[2] =0;	
		}
	}
	else
	{
		if(SW6206_all_Data.IRQ_Data&0x04)
		{
			if(SW6206_all_Data.BatTure_Data/100)
			{
				LED_NUM[2] =Led_Tab[SW6206_all_Data.BatTure_Data/100];
			}
			else
			{
				LED_NUM[2] = 0;
			}
			LED_NUM[1] =Led_Tab[(SW6206_all_Data.BatTure_Data%100/10)];
			LED_NUM[0] =Led_Tab[SW6206_all_Data.BatTure_Data%10]|0x80;								//0x80为%
			if(B_500ms_Flag)
			{
				LED_NUM[0] &=~0x7f;
			}		
		}
		else
		{
			if(device_all_data.R_DisPlay_Time)					
			{
				if(device_all_data.B_NTC_Err)							//显示E1
				{
					LED_NUM[0] =Led_Tab[1];
					LED_NUM[1] =Led_Tab[0x0E];		
					LED_NUM[2] =0;	
				}
				else if(device_all_data.B_NTC_CoolErr)					//显示E3
				{
					LED_NUM[0] =Led_Tab[3];
					LED_NUM[1] =Led_Tab[0x0E];		
					LED_NUM[2] =0;
				}
				else if(device_all_data.B_NTC_HotErr)					//显示E4
				{
					LED_NUM[0] =Led_Tab[4];
					LED_NUM[1] =Led_Tab[0x0E];		
					LED_NUM[2] =0;	
				}
				if(device_all_data.R_Work_Mold==1)		//显示C
				{
					LED_NUM[0] =0;
					LED_NUM[1] =Led_Tab[0x0C];		
					LED_NUM[2] =0;	
				}
				if(device_all_data.R_Work_Mold==2)		//显示F
				{
					LED_NUM[0] =0;
					LED_NUM[1] =Led_Tab[0x0F];		
					LED_NUM[2] =0;	
				}	
				if(device_all_data.R_Work_Mold==3)		//显示H
				{
					LED_NUM[0] =0;
					LED_NUM[1] =Led_Tab[0x10];		
					LED_NUM[2] =0;	
				}					
			}
			else
			{
				LED_NUM[0] =0;
				LED_NUM[1] =0;		
				LED_NUM[2] =0;
//				osal_set_event(APP_TaskID,EVENT_Sleep);
			}
		}
	}
}

u8 l_judge_onoff(u8 week,u8 hour,u8 minute)  //判断工作星期  小时  分钟
{
	u8 temp_result = 0;
	switch(lxl_time_rtc_data.dev_week )
	{
		case 1:
			if((week &0x01)&&(lxl_time_rtc_data.dev_hour == hour) &&(lxl_time_rtc_data.dev_minute == minute))
			{
				temp_result = 1; 
			}
			else
			{
				temp_result = 0;
			}
			break;
		case 2:
			if((week &0x02)&&(lxl_time_rtc_data.dev_hour == hour) &&(lxl_time_rtc_data.dev_minute == minute))
			{
				temp_result = 1; 
			}
			else
			{
				temp_result = 0;
			}
			break;
		case 3:
			if((week &0x04)&&(lxl_time_rtc_data.dev_hour == hour) &&(lxl_time_rtc_data.dev_minute == minute))
			{
				temp_result = 1; 
			}
			else
			{
				temp_result = 0;
			}
			break;
		case 4:
			if((week &0x08)&&(lxl_time_rtc_data.dev_hour == hour) &&(lxl_time_rtc_data.dev_minute == minute))
			{
				temp_result = 1; 
			}
			else
			{
				temp_result = 0;
			}
			break;
		case 5:
			if((week &0x10)&&(lxl_time_rtc_data.dev_hour == hour) &&(lxl_time_rtc_data.dev_minute == minute))
			{
				temp_result = 1; 
			}
			else
			{
				temp_result = 0;
			}
			break;
		case 6:
			if((week &0x20)&&(lxl_time_rtc_data.dev_hour == hour) &&(lxl_time_rtc_data.dev_minute == minute))
			{
				temp_result = 1; 
			}
			else
			{
				temp_result = 0;
			}
			break;
		case 7:
			if((week &0x40)&&(lxl_time_rtc_data.dev_hour == hour) &&(lxl_time_rtc_data.dev_minute == minute))
			{
				temp_result = 1; 
			}
			else
			{
				temp_result = 0;
			}
			break;
		default:
			break;
	}
	return temp_result;
}
//-------------------定时判断-------------------------//
void l_judge_time(void)   //判断定时开关
{
	//lxl_time_rtc_data.dev_week lxl_time_rtc_data.dev_hour, lxl_time_rtc_data.dev_minute
	// 0:当前定时开关  1:工作星期  2:小时 3:分钟   4:该状态执行一次
	//----------定时开判断-------------//
	if(device_all_data.device_time_on[0])
	{
		if(l_judge_onoff(device_all_data.device_time_on[1],
                      device_all_data.device_time_on[2],
                      device_all_data.device_time_on[3]))
		{
			if(!device_all_data.device_time_on[4]) //该状态执行一次
			{  
				device_all_data.device_time_on[4] = 1;
				device_all_data.device_onoff_flag = 1;
//				LOG("OFF\n");
			}
		}
		else
		{
			device_all_data.device_time_on[4] = 0;
		}
	}
	//----------定时关判断-------------//
	if(device_all_data.device_time_off[0] == 1)
	{
		if(l_judge_onoff(device_all_data.device_time_off[1],
                      device_all_data.device_time_off[2],
                      device_all_data.device_time_off[3]))
		{
			if(!device_all_data.device_time_off[4])	//该状态执行一次
			{  
				device_all_data.device_time_off[4] = 1;
				device_all_data.device_onoff_flag = 0;
//				LOG("ON\n");
			}
		}
		else
		{
			device_all_data.device_time_off[4] = 0;
		}
	}
}

//--------------------BLE上报数据---------------------//
static u8 data_tail = 0;
static u8 data_head = 0;
static u8 data_send_buff[100] = {0};
void send_type_request(u8 send_type)
{
	data_head++;
	data_head%=100;
	data_send_buff[data_head] = send_type;
}
void command_send_app_data(void)					//BLE->APP
{
	u8 send_data[20] = {0};
	if(data_tail != data_head)
	{
		data_tail++;
		data_tail %=100;
		switch(data_send_buff[data_tail])
		{
			case 0:  //开关灯状态
       
				send_data[0] = 0xcc;
				send_data[1] = 0x01;
				send_data[2] = 0x01;
				send_data[3] = device_all_data.device_onoff_flag;
				send_data[4] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,5,send_data);		
				break;
			case 1:  //当前设备线性

				send_data[0] = 0xcc;
				send_data[1] = 0x02;
				send_data[2] = 0x01;
				send_data[3] = 0x55;
				send_data[4] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,5,send_data);
				 
				break;
			case 2:  //灯珠总数

				send_data[0] = 0xcc;
				send_data[1] = 0x03;
				send_data[2] = 0x02;
				send_data[3] = 0xff;
				send_data[4] = 0xff;
				send_data[5] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,6,send_data);
				 
				break;
			case 3:  //调色

				send_data[0] = 0xcc;
				send_data[1] = 0x04;
				send_data[2] = 0x06;
				send_data[3] = 8;
				send_data[4] = 0xff;
				send_data[5] = 8;
				send_data[6] = 0xff;
				send_data[7] = 0x00;
				send_data[8] = 0x00;
				send_data[9] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,10,send_data);
				 
				break;
			case 4:  //亮度

				send_data[0] = 0xcc;
				send_data[1] = 0x05;
				send_data[2] = 0x06;
				send_data[3] = 8;
				send_data[4] = 0xff;
				send_data[5] = 0;
				send_data[6] = 0;
				send_data[7] = 0;
				send_data[8] = 0;
				send_data[9] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,10,send_data);
				 
				break;
			case 5:  //模式

				send_data[0] = 0xcc;
				send_data[1] = 0x06;
				send_data[2] = 0x02;
				send_data[3] = 8;
				send_data[4] = 0xff;
				send_data[5] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,6,send_data);
				 
				break;
			case 6:  //模式方向

				send_data[0] = 0xcc;
				send_data[1] = 0x06;
				send_data[2] = 1;
				send_data[3] = 0;
				send_data[4] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,5,send_data);
				 
				break;
			case 7:  //速度
				break;
			case 8:  //定时开

				send_data[0] = 0xcc;
				send_data[1] = 0x0D;
				send_data[2] = 0x06;
				send_data[3] = 0x01;  //ID
				send_data[4] = 0x01;  //定时开
				send_data[5] = device_all_data.device_time_on[0]; //开关
				send_data[6] = device_all_data.device_time_on[1]; //星期
				send_data[7] = device_all_data.device_time_on[2]; //小时
				send_data[8] = device_all_data.device_time_on[3]; //分钟
				send_data[9] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,10,send_data);

				break; 
			case 9:  //定时关

				send_data[0] = 0xcc;
				send_data[1] = 0x0D;
				send_data[2] = 0x06;
				send_data[3] = 0x01;  //ID
				send_data[4] = 0x00;  //定时开
				send_data[5] = device_all_data.device_time_off[0]; //开关
				send_data[6] = device_all_data.device_time_off[1]; //星期
				send_data[7] = device_all_data.device_time_off[2]; //小时
				send_data[8] = device_all_data.device_time_off[3]; //分钟
				send_data[9] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,10,send_data);

				break; 
			default:
				break;
		}
	}
}

//--------------------APP数据判断---------------------//
void l_judge_app_data(u8 app_data[])			//app->BLE
{
	u32 time_data = 0;
	//----------开关灯状态---------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x01 &&app_data[2]==0x01 &&app_data[4]== 0x55)
	{  
		device_all_data.device_onoff_flag = app_data[3] ;   //开关
		LOG("APP_ONOFF=%d\n",device_all_data.device_onoff_flag );
	}
      
	//------------APP更新时间戳----------//
	if(app_data[0] == 0xbc && app_data[1] == 0x0b &&app_data[2]==0x07 &&app_data[10] == 0x55)
	{
		lxl_time_rtc_data.dev_year  = app_data[3]<<8|app_data[4];
		lxl_time_rtc_data.dev_month = app_data[5];
		lxl_time_rtc_data.dev_day   = app_data[6];
		lxl_time_rtc_data.dev_hour  = app_data[7];
		lxl_time_rtc_data.dev_minute = app_data[8];
		lxl_time_rtc_data.dev_second = app_data[9];
		time_data = my_rtc_mktime(lxl_time_rtc_data);
		lxl_set_rtc_clock_data_func(time_data); 
	}
	//-----------定时查询----------------//  
	if(app_data[0] == 0xbc && app_data[1] == 0x0c &&app_data[2]==0x01 &&app_data[4]== 0x55)
	{
		if(app_data[3]== 0x01)
		{
			send_type_request(8);
			send_type_request(9);
		}
	}
	//-----------定时设置----------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x0d &&app_data[2]==0x06 &&app_data[9] == 0x55)
	{
		//app_data[3]  组号ID
		if(app_data[4])	//设置定时开
		{  
			device_all_data.device_time_on[0] = app_data[5];  //开关
			device_all_data.device_time_on[1] = app_data[6];  //星期
			device_all_data.device_time_on[2] = app_data[7];  //小时
			device_all_data.device_time_on[3] = app_data[8];  //分钟 
			device_all_data.device_time_on[4] = 0;    
		}
		else	//设置定时关
		{            
			device_all_data.device_time_off[0] = app_data[5];  //开关
			device_all_data.device_time_off[1] = app_data[6];  //星期
			device_all_data.device_time_off[2] = app_data[7];  //小时
			device_all_data.device_time_off[3] = app_data[8];  //分钟  
			device_all_data.device_time_off[4] = 0;  
		}
	}  
	//-----------制热模式------------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x0f &&app_data[2]==0x01 &&app_data[4] == 0x55)
	{
		if(device_all_data.device_onoff_flag)
		{

		}
	}

	//-----------制冷模式---------------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x11 &&app_data[2]==0x01 &&app_data[4] == 0x55)
	{

	}	

}
