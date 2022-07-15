/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename��other_function.c
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
bool B_500ms_Flag=0;							//�����¶�
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

void HW_timer_callback(uint8 evt)       //ʱ���ۼ�
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

void HW_time_100us_stop(void)      //�ض϶�ʱ��
{
	hal_timer_stop(AP_TIMER_ID_5) ;
}

void HW_timer_100us(void)     //��ʱ������
{ 
	hal_timer_init(HW_timer_callback); 
	hal_timer_set(AP_TIMER_ID_5,100); 
} 


struct_PID_ALL PID_Value =
{
	.LiEKValue ={0},		//��ֵ���棬�����ͷ����Ĳ�ֵ
	.uEKFlag ={0},			//���ţ�1���Ӧ��Ϊ������0���Ӧ��Ϊ����
	.uKP_Coe =0,			//����ϵ��
	.uKI_Coe =0,			//���ֳ���
	.uKD_Coe =0,			//΢�ֳ���
	.iPriValue =0,			//��һʱ��ֵ
	.iSetValue =0,			//�趨ֵ
	.iCurValue =0,			//ʵ��ֵ
};

void PID_Operation(void)
{
	u32 temp[3]={0};	//�м���ʱ����
	u32 PostSum=0;		//����֮��
	u32 NegSum=0;		//����֮��
	
	if(PID_Value.iSetValue > PID_Value.iCurValue)
	{
		if(PID_Value.iSetValue-PID_Value.iCurValue>10)						//ƫ��ֵ����10��
		{
			PID_Value.iPriValue=100;										//ȫ�����
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
			if(PostSum>NegSum)							//�жϿ������Ƿ�Ϊ����
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
		Hot_Open();				//�򿪷���
	}
	else
	{
		Hot_Cool_Close();		//�رշ���
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
*1S����
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
	if(device_all_data.R_DisPlay_Time)							//��ʾʱ��
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
*���ȵ�43��ά��1min,�¶Ƚ���40�ȣ�ά��10���ӣ��¶�������43�㣬ά��5min
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
	PID_Output();					//�����¶�
}
/***************************************************************
*
*
*���ȵ�47��ά��1min,�¶Ƚ���45�ȣ�ά��10���ӣ��¶Ƚ���40�㣬ά��5min
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
	PID_Output();					//�����¶�
}
/***************************************************************
*
*
*���ȵ�53��ά��1min,�¶Ƚ���50�ȣ�ά��10���ӣ��¶Ƚ���46�㣬ά��5min
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
	PID_Output();					//�����¶�	
}
/***************************************************************
*
*
*����ģʽ
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
		if(device_all_data.B_Cool_Delay)			//����Ƭ50%
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
			if(device_all_data.R_Work_Level ==1)				//��ʾC1
			{
				LED_NUM[0] =Led_Tab[1];
				LED_NUM[1] =Led_Tab[0x0C];	
				LED_NUM[2] =0;					
			}
			if(device_all_data.R_Work_Level ==2)				//��ʾC2
			{
				LED_NUM[0] =Led_Tab[2];
				LED_NUM[1] =Led_Tab[0x0C];		
				LED_NUM[2] =0;	
			}
			if(device_all_data.R_Work_Level ==3)				//��ʾC3
			{
				LED_NUM[0] =Led_Tab[3];
				LED_NUM[1] =Led_Tab[0x0C];		
				LED_NUM[2] =0;					
			}			
		}
		else if((device_all_data.R_Work_Mold==2)&&(device_all_data.R_DisPlay_Time>26))
		{
			if(device_all_data.R_Work_Level ==1)				//��ʾF1
			{
				LED_NUM[0] =Led_Tab[1];
				LED_NUM[1] =Led_Tab[0x0F];		
				LED_NUM[2] =0;	
			}
			if(device_all_data.R_Work_Level ==2)				//��ʾF2
			{
				LED_NUM[0] =Led_Tab[2];
				LED_NUM[1] =Led_Tab[0x0F];		
				LED_NUM[2] =0;	
			}
			if(device_all_data.R_Work_Level ==3)				//��ʾF3
			{
				LED_NUM[0] =Led_Tab[3];
				LED_NUM[1] =Led_Tab[0x0F];		
				LED_NUM[2] =0;
			}
		}
		else if((device_all_data.R_Work_Mold==3)&&(device_all_data.R_DisPlay_Time>26))
		{
			if(device_all_data.R_Work_Level ==1)				//��ʾH1
			{
				LED_NUM[0] =Led_Tab[1];
				LED_NUM[1] =Led_Tab[0x10];		
				LED_NUM[2] =0;				
			}
			if(device_all_data.R_Work_Level ==2)				//��ʾH2
			{
				LED_NUM[0] =Led_Tab[2];
				LED_NUM[1] =Led_Tab[0x10];		
				LED_NUM[2] =0;						
			}
			if(device_all_data.R_Work_Level ==3)				//��ʾH3
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
			LED_NUM[0] =Led_Tab[SW6206_all_Data.BatTure_Data%10]|0x80;								//0x80Ϊ%
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
			LED_NUM[0] =Led_Tab[SW6206_all_Data.BatTure_Data%10]|0x80;								//0x80Ϊ%
			if(B_500ms_Flag)
			{
				LED_NUM[0] &=~0x7f;
			}		
		}
		else
		{
			if(device_all_data.R_DisPlay_Time)					
			{
				if(device_all_data.B_NTC_Err)							//��ʾE1
				{
					LED_NUM[0] =Led_Tab[1];
					LED_NUM[1] =Led_Tab[0x0E];		
					LED_NUM[2] =0;	
				}
				else if(device_all_data.B_NTC_CoolErr)					//��ʾE3
				{
					LED_NUM[0] =Led_Tab[3];
					LED_NUM[1] =Led_Tab[0x0E];		
					LED_NUM[2] =0;
				}
				else if(device_all_data.B_NTC_HotErr)					//��ʾE4
				{
					LED_NUM[0] =Led_Tab[4];
					LED_NUM[1] =Led_Tab[0x0E];		
					LED_NUM[2] =0;	
				}
				if(device_all_data.R_Work_Mold==1)		//��ʾC
				{
					LED_NUM[0] =0;
					LED_NUM[1] =Led_Tab[0x0C];		
					LED_NUM[2] =0;	
				}
				if(device_all_data.R_Work_Mold==2)		//��ʾF
				{
					LED_NUM[0] =0;
					LED_NUM[1] =Led_Tab[0x0F];		
					LED_NUM[2] =0;	
				}	
				if(device_all_data.R_Work_Mold==3)		//��ʾH
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

u8 l_judge_onoff(u8 week,u8 hour,u8 minute)  //�жϹ�������  Сʱ  ����
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
//-------------------��ʱ�ж�-------------------------//
void l_judge_time(void)   //�ж϶�ʱ����
{
	//lxl_time_rtc_data.dev_week lxl_time_rtc_data.dev_hour, lxl_time_rtc_data.dev_minute
	// 0:��ǰ��ʱ����  1:��������  2:Сʱ 3:����   4:��״ִ̬��һ��
	//----------��ʱ���ж�-------------//
	if(device_all_data.device_time_on[0])
	{
		if(l_judge_onoff(device_all_data.device_time_on[1],
                      device_all_data.device_time_on[2],
                      device_all_data.device_time_on[3]))
		{
			if(!device_all_data.device_time_on[4]) //��״ִ̬��һ��
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
	//----------��ʱ���ж�-------------//
	if(device_all_data.device_time_off[0] == 1)
	{
		if(l_judge_onoff(device_all_data.device_time_off[1],
                      device_all_data.device_time_off[2],
                      device_all_data.device_time_off[3]))
		{
			if(!device_all_data.device_time_off[4])	//��״ִ̬��һ��
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

//--------------------BLE�ϱ�����---------------------//
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
			case 0:  //���ص�״̬
       
				send_data[0] = 0xcc;
				send_data[1] = 0x01;
				send_data[2] = 0x01;
				send_data[3] = device_all_data.device_onoff_flag;
				send_data[4] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,5,send_data);		
				break;
			case 1:  //��ǰ�豸����

				send_data[0] = 0xcc;
				send_data[1] = 0x02;
				send_data[2] = 0x01;
				send_data[3] = 0x55;
				send_data[4] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,5,send_data);
				 
				break;
			case 2:  //��������

				send_data[0] = 0xcc;
				send_data[1] = 0x03;
				send_data[2] = 0x02;
				send_data[3] = 0xff;
				send_data[4] = 0xff;
				send_data[5] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,6,send_data);
				 
				break;
			case 3:  //��ɫ

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
			case 4:  //����

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
			case 5:  //ģʽ

				send_data[0] = 0xcc;
				send_data[1] = 0x06;
				send_data[2] = 0x02;
				send_data[3] = 8;
				send_data[4] = 0xff;
				send_data[5] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,6,send_data);
				 
				break;
			case 6:  //ģʽ����

				send_data[0] = 0xcc;
				send_data[1] = 0x06;
				send_data[2] = 1;
				send_data[3] = 0;
				send_data[4] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,5,send_data);
				 
				break;
			case 7:  //�ٶ�
				break;
			case 8:  //��ʱ��

				send_data[0] = 0xcc;
				send_data[1] = 0x0D;
				send_data[2] = 0x06;
				send_data[3] = 0x01;  //ID
				send_data[4] = 0x01;  //��ʱ��
				send_data[5] = device_all_data.device_time_on[0]; //����
				send_data[6] = device_all_data.device_time_on[1]; //����
				send_data[7] = device_all_data.device_time_on[2]; //Сʱ
				send_data[8] = device_all_data.device_time_on[3]; //����
				send_data[9] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,10,send_data);

				break; 
			case 9:  //��ʱ��

				send_data[0] = 0xcc;
				send_data[1] = 0x0D;
				send_data[2] = 0x06;
				send_data[3] = 0x01;  //ID
				send_data[4] = 0x00;  //��ʱ��
				send_data[5] = device_all_data.device_time_off[0]; //����
				send_data[6] = device_all_data.device_time_off[1]; //����
				send_data[7] = device_all_data.device_time_off[2]; //Сʱ
				send_data[8] = device_all_data.device_time_off[3]; //����
				send_data[9] = 0xaa;
				lxl_SimpleProfile_SetParameter(1,10,send_data);

				break; 
			default:
				break;
		}
	}
}

//--------------------APP�����ж�---------------------//
void l_judge_app_data(u8 app_data[])			//app->BLE
{
	u32 time_data = 0;
	//----------���ص�״̬---------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x01 &&app_data[2]==0x01 &&app_data[4]== 0x55)
	{  
		device_all_data.device_onoff_flag = app_data[3] ;   //����
		LOG("APP_ONOFF=%d\n",device_all_data.device_onoff_flag );
	}
      
	//------------APP����ʱ���----------//
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
	//-----------��ʱ��ѯ----------------//  
	if(app_data[0] == 0xbc && app_data[1] == 0x0c &&app_data[2]==0x01 &&app_data[4]== 0x55)
	{
		if(app_data[3]== 0x01)
		{
			send_type_request(8);
			send_type_request(9);
		}
	}
	//-----------��ʱ����----------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x0d &&app_data[2]==0x06 &&app_data[9] == 0x55)
	{
		//app_data[3]  ���ID
		if(app_data[4])	//���ö�ʱ��
		{  
			device_all_data.device_time_on[0] = app_data[5];  //����
			device_all_data.device_time_on[1] = app_data[6];  //����
			device_all_data.device_time_on[2] = app_data[7];  //Сʱ
			device_all_data.device_time_on[3] = app_data[8];  //���� 
			device_all_data.device_time_on[4] = 0;    
		}
		else	//���ö�ʱ��
		{            
			device_all_data.device_time_off[0] = app_data[5];  //����
			device_all_data.device_time_off[1] = app_data[6];  //����
			device_all_data.device_time_off[2] = app_data[7];  //Сʱ
			device_all_data.device_time_off[3] = app_data[8];  //����  
			device_all_data.device_time_off[4] = 0;  
		}
	}  
	//-----------����ģʽ------------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x0f &&app_data[2]==0x01 &&app_data[4] == 0x55)
	{
		if(device_all_data.device_onoff_flag)
		{

		}
	}

	//-----------����ģʽ---------------------//
	if(app_data[0] == 0xbc && app_data[1] == 0x11 &&app_data[2]==0x01 &&app_data[4] == 0x55)
	{

	}	

}
