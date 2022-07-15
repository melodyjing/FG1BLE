/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：time_rtc_fun.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains for time rtc fun. 
************************************************************************/
#include "all_gpio_init.h"
#include "OSAL_Clock.h"


lxl_time_rtc_all lxl_time_rtc_data = {
	.dev_time_at=0,  //当前时间戳

	.dev_year=0,     //当前时间"年"
	.dev_month=0,    //当前时间"月"
	.dev_day=0,      //当前时间"日"
	.dev_week=0,     //当前时间"周"
				
	.dev_hour=0,     //当前时间"时"
	.dev_minute=0,   //当前时间"分"
	.dev_second=0,   //当前时间"秒"
};  //当前rtc的定时器数据
RTC8563_Alarm_Data my_alarm_data_1[7]  ;   //闹钟结构体

static u32 my_rtc_mon_yday[2][12] ={
	{0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
	{0,31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335},
};
const char my_rtc_Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//**************************************************************************************************
//						《当前年月日算出周几》(Year_h：年高位（20）, Year_l:年低位(year - 2000)
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
//						《根据U32秒算出当前年月日》(time:U32 秒, time_count_temp *t:年月日)
//*************************************************************************************************
void my_rtc_localtime(u32 time,lxl_time_rtc_all *t){
	u32 Pass4year;
	u32 hours_per_year;
//	if(time < 0)
//	{
//		time = 0;
//	}
	t->dev_second=(u32)(time % 60);  //取秒时间
	time /= 60;
	t->dev_minute=(u32)(time % 60);  //取分钟时间
	time /= 60;
	Pass4year=((unsigned int)time / (1461L * 24L));  //取过去多少个四年，每四年有 1461*24 小时
	t->dev_year=(Pass4year << 2) + 1970;  //计算年份
	time %= 1461L * 24L;    //四年中剩下的小时数
	for (;;)    	        //校正闰年影响的年份，计算一年中剩下的小时数
	{
		hours_per_year = 365 * 24;   //一年的小时数
		if ((t->dev_year & 3) == 0)	 //判断闰年 	
		{  
			hours_per_year += 24;    //是闰年，一年则多24小时，即一天
		}
		if (time < hours_per_year)
		{
			break;
		}
		t->dev_year++;
		time -= hours_per_year;
	}
	t->dev_hour=(int)(time % 24)+8;   //小时数
	time /= 24;  //一年中剩下的天数
	time++;      //假定为闰年
	if((t->dev_year & 3) == 0)  //校正闰年的误差，计算月份，日期
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
	//计算月日
	for (t->dev_month = 0; my_rtc_Days[t->dev_month] < time;t->dev_month++)
	{
		time -= my_rtc_Days[t->dev_month];
	}
	t->dev_day = (int)(time);
	t->dev_month+=1;
	return;
}
//**************************************************************************************************
//						《校验是否为闰年》
//*************************************************************************************************
int my_rtc_isleap(int year)
{
	return (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0);
}
//**************************************************************************************************
//						《根据当前年月日算出U32秒》
//*************************************************************************************************
u32 my_rtc_mktime(lxl_time_rtc_all dt)
{
	u32 result;
	int i =0;
	// 以平年时间计算的秒数
	result = (dt.dev_year - 1970) * 365 * 24 * 3600 +
	(my_rtc_mon_yday[my_rtc_isleap(dt.dev_year)][dt.dev_month-1] + dt.dev_day - 1) * 24 * 3600 +\
	dt.dev_hour * 3600 + dt.dev_minute * 60 + dt.dev_second;

	for(i=1970; i < dt.dev_year; i++)		// 加上闰年的秒数
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


void lxl_set_rtc_clock_data_func(u32 time_clock)   //设置系统时间
{
	osal_setClock(time_clock - LXL_2000_UNIX_TIME) ;
	lxl_get_rtc_time_data_func() ;
}	

void lxl_get_rtc_time_data_func(void)    //获取系统时间
{
	u32 time_temp = 0 ;
	time_temp = osal_getClock() ;             ///  就获取 UTCTime  单位为秒
	if(time_temp)
	{
		lxl_time_rtc_data.dev_time_at = time_temp ;
		my_rtc_localtime(time_temp,&lxl_time_rtc_data) ;
		lxl_time_rtc_data.dev_week =my_rtc_week_count(lxl_time_rtc_data.dev_year,lxl_time_rtc_data.dev_month,lxl_time_rtc_data.dev_day) ;
	}
}


 

