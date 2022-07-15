/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename��time_rtc_fun.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains for time rtc fun. 
************************************************************************/
#include "all_gpio_init.h"
#include "OSAL_Clock.h"


lxl_time_rtc_all lxl_time_rtc_data = {
	.dev_time_at=0,  //��ǰʱ���

	.dev_year=0,     //��ǰʱ��"��"
	.dev_month=0,    //��ǰʱ��"��"
	.dev_day=0,      //��ǰʱ��"��"
	.dev_week=0,     //��ǰʱ��"��"
				
	.dev_hour=0,     //��ǰʱ��"ʱ"
	.dev_minute=0,   //��ǰʱ��"��"
	.dev_second=0,   //��ǰʱ��"��"
};  //��ǰrtc�Ķ�ʱ������
RTC8563_Alarm_Data my_alarm_data_1[7]  ;   //���ӽṹ��

static u32 my_rtc_mon_yday[2][12] ={
	{0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
	{0,31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335},
};
const char my_rtc_Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//**************************************************************************************************
//						����ǰ����������ܼ���(Year_h�����λ��20��, Year_l:���λ(year - 2000)
//*************************************************************************************************
u8 my_rtc_week_count(u32 Year, u8 Month, u8 Day){
	u8 Year_h = 0 ;
	u8 Year_l = 0 ;
	u32 year_temp = my_16_switch_10_func(Year) ;
	Year_h = year_temp/100 ;
	Year_l = year_temp%100 ;
	//LOG("Year_h:%d---%d---%d---%d\n",Year,year_temp,Year_h,Year_l) ;	
    if(Month<2)
	{
        Month = Month+12 ;
        Year_l = Year_l-1 ;
    }
    u32 temp = 0 ;
    temp = Year_l+(Year_l/4)+(Year_h/4)-(2*Year_h)+((26*(Month+1))/10)+Day-1 ;
    temp = temp%7 ;
    if(temp==0) temp = 7 ;
    return temp ;
}
//**************************************************************************************************
//						������U32�������ǰ�����ա�(time:U32 ��, time_count_temp *t:������)
//*************************************************************************************************
void my_rtc_localtime(u32 time,lxl_time_rtc_all *t){
	u32 Pass4year;
	u32 hours_per_year;
//	if(time < 0)
//	{
//		time = 0;
//	}
	t->dev_second=(u32)(time % 60);  //ȡ��ʱ��
	time /= 60;
	t->dev_minute=(u32)(time % 60);  //ȡ����ʱ��
	time /= 60;
	Pass4year=((unsigned int)time / (1461L * 24L));  //ȡ��ȥ���ٸ����꣬ÿ������ 1461*24 Сʱ
	t->dev_year=(Pass4year << 2) + 1970;  //�������
	time %= 1461L * 24L;    //������ʣ�µ�Сʱ��
	for (;;)    	        //У������Ӱ�����ݣ�����һ����ʣ�µ�Сʱ��
	{
		hours_per_year = 365 * 24;   //һ���Сʱ��
		if ((t->dev_year & 3) == 0)	 //�ж����� 	
		{  
			hours_per_year += 24;    //�����꣬һ�����24Сʱ����һ��
		}
		if (time < hours_per_year)
		{
			break;
		}
		t->dev_year++;
		time -= hours_per_year;
	}
	t->dev_hour=(int)(time % 24)+8;   //Сʱ��
	time /= 24;  //һ����ʣ�µ�����
	time++;      //�ٶ�Ϊ����
	if((t->dev_year & 3) == 0)  //У��������������·ݣ�����
	{
		if (time > 60)
		{
			time--;
		}
		else
		{
			if (time == 60)
			{
				t->dev_month = 1;
				t->dev_day = 29;
				return ;
			}
		}
	}
	//��������
	for (t->dev_month = 0; my_rtc_Days[t->dev_month] < time;t->dev_month++)
	{
		time -= my_rtc_Days[t->dev_month];
	}
	t->dev_day = (int)(time);
	t->dev_month+=1;
	return;
}
//**************************************************************************************************
//						��У���Ƿ�Ϊ���꡷
//*************************************************************************************************
int my_rtc_isleap(int year)
{
	return (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0);
}
//**************************************************************************************************
//						�����ݵ�ǰ���������U32�롷
//*************************************************************************************************
u32 my_rtc_mktime(lxl_time_rtc_all dt)
{
	u32 result;
	int i =0;
	// ��ƽ��ʱ����������
	result = (dt.dev_year - 1970) * 365 * 24 * 3600 +
	(my_rtc_mon_yday[my_rtc_isleap(dt.dev_year)][dt.dev_month-1] + dt.dev_day - 1) * 24 * 3600 +\
	dt.dev_hour * 3600 + dt.dev_minute * 60 + dt.dev_second;

	for(i=1970; i < dt.dev_year; i++)		// �������������
	{      
		if(my_rtc_isleap(i)) result += 24 * 3600;
	}
	result = result - (8*3600) ;
	return(result);
}
u32 my_10_switch_16_func(u32 data)
{
	u32 temp = 1 ;
	u32 data_temp = data%10*temp;
	do{
		temp *= 16 ;
		data = data/10 ;
		data_temp += data%10*temp;
	}while(data/10);
	return data_temp;
}
u32 my_16_switch_10_func(u32 data)
{
	u32 temp = 1 ;
	u32 data_temp = data%10*temp;
	do{
		temp *= 10 ;
		data = data/10 ;
		data_temp += data%10*temp;
	}while(data/10);
	return data_temp;
}


void lxl_set_rtc_clock_data_func(u32 time_clock)   //����ϵͳʱ��
{
	osal_setClock(time_clock - LXL_2000_UNIX_TIME) ;
	lxl_get_rtc_time_data_func() ;
}	

void lxl_get_rtc_time_data_func(void)    //��ȡϵͳʱ��
{
	u32 time_temp = 0 ;
	time_temp = osal_getClock() ;             ///  �ͻ�ȡ UTCTime  ��λΪ��
	if(time_temp)
	{
		lxl_time_rtc_data.dev_time_at = time_temp ;
		my_rtc_localtime(time_temp,&lxl_time_rtc_data) ;
		lxl_time_rtc_data.dev_week =my_rtc_week_count(lxl_time_rtc_data.dev_year,lxl_time_rtc_data.dev_month,lxl_time_rtc_data.dev_day) ;
	}
}


 

