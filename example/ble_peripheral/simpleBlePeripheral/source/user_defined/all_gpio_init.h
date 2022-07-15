#ifndef __LXL_GPIO_ALL_INIT_H__
#define __LXL_GPIO_ALL_INIT_H__


#include <string.h>
#include <stdlib.h>
#include "OSAL_Tasks.h"
#include "types.h"
#include "rom_sym_def.h"
#include "types.h"
#include "gpio.h"
#include "clock.h"
#include "log.h"
#include "error.h"
#include "OSAL.h"
#include "pwrmgr.h"
#include "bcomdef.h"
#include "OSAL.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gatt_profile_uuid.h"
#include "linkdb.h"
#include "gattservapp.h"
#include "peripheral.h"
#include "bcomdef.h"
#include "linkdb.h"
#include "osal.h"
#include "gatt.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "peripheral.h"
#include "gapbondmgr.h"
#include "gatt_profile_uuid.h"
#include "log.h"
#include "adc.h"
#include "pwm.h"
#include "uart.h"
#include "Timer.h"
#include "SBPPROFILE_OTA.h"

#include "spi.h"
#include "dma.h"
#include "spiflash.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
//============================================================//
#define	 LED_GPIO_A					P14		//5
#define	 LED_GPIO_B					P0
#define  LED_GPIO_C					P34
#define	 LED_GPIO_D					P33
#define  LED_GPIO_E					P23


#define	 KEY_SW_1					P9
#define	 KEY_SW_2					P32

#define	 Cool_Contorl_A				P7			//制冷片A相
#define	 Cool_Contorl_B				P31			//制冷片B相

#define	 SW6206_IRQ					P11
#define	 SW6206_SCL					P2
#define	 SW6206_SDA					P3


#define	 Fan_GPIO					P18

#define	 Speech_Contorl				P20

#define	 NTC1_CHANNEL				P24
#define	 NTC2_CHANNEL				P15



#define SW6206_Device			    0x78

#define KeyConf_REG					0x03	//按键配置
#define ShortKey_REG				0x04	//短按事件
#define	IRQ_REG						0x06	//灯显状态	
#define	Pullout_REG					0x08	//端口拔出
#define	Pullin_REG					0x09	//端口插入
#define	BatErr_REG					0x0A	//电池异常
#define	SysErr_REG					0x0B	//系统异常
#define Status_REG					0x0C	//系统状态
#define PDContorl_REG				0x0F	//快充协议
#define	ADCConf_REG					0x12	//ADC配置
#define ADCH_REG					0x13	//ADC高八位
#define ADCL_REG					0x14	//ADC低四位
#define OutInControl_REG			0x18	//输入输出控制使能
#define PortEvent_REG				0x19	//端口事件触发
#define QCConfig0_REG				0x1A	//快充配置0
#define QCConfig1_REG				0x1B	//快充配置1
#define QCConfig2_REG				0x1C	//快充配置2
#define QCConfig3_REG				0x1D	//快充配置3
#define QCConfig4_REG				0x1E	//快充配置4  BIT4:0支持边充边放，1不支持边充边放，充电优先，BIT1,C口QC source使能 0使能
#define QCDirect_REG				0x1F	//快充指示
#define OVP_REG						0x21	//VDD过压事件，BIT5
#define CMD_REG						0x22	//PD命令， BIT3-0:1为重置命令
#define TypeCconfig_REG				0x28	//TypeC配置 BIT3-2:0 Strong drp,1 sink 2 source
#define TypeC_REG					0x29	//TypeC指示 BIT1-0: 1sink 2source
#define PDConfig0_REG				0x2A	//PD配置0
#define PDConfig1_REG				0x2B	//PD配置1
#define PDConfig2_REG				0x2C	//PD配置2
#define PD_REG						0x2D	//多场景PD控制 BIT5:0 广播5VPDO,1不使能，BIT2：低压SCP 0使能 BIT0：三星1.2V使能 0使能
#define SCurr_REG					0x2E	//小电流充电控制 BIT4: 1进入或退出小电流充电，BIT0:1处于小电流充电状态
#define Check0_REG					0x30	//接入拔出检测配置0
#define Check1_REG					0x31	//接入拔出检测配置1
#define Wireless_REG				0x32	//无线充配置
#define SCurrOut_REG				0x33	//小电流充电配置
#define Boost0_REG					0x40	//Boost0配置
#define Boost1_REG					0x41	//Boost1配置
#define Chager0_REG					0x42	//Chager0配置
#define Chager1_REG					0x43	//Chager1配置
#define Chager2_REG					0x44	//Chager2配置
#define Chager3_REG					0x45	//Chager3配置
#define Chager4_REG					0x46	//Chager4配置
#define NTC0_REG					0x47	//NTC配置0
#define NTC1_REG					0x48	//NTC配置1
#define Temp_REG					0x49	//温度设置
#define Bat_REG						0x72	//最终电量
#define BatCapL_REG					0x73	//电池容量低8位
#define BatCapH_REG					0x74	//电池容量高4位
#define Charge_REG					0x7A	//充电控制，BIT5：0正常值，1,比正常值低0.1V，BIT4:充电电流减低设置使能，1使能，通过BIT3设置电流，1为1A，0为0.5A
#define BatTure_REG					0x7E	//最终处理电量

/*********RF Contorl*************************/
#define DEFINE_RF					0

#if 0
#define  SET_KEY_CHANNEL_1			P10
#define  SET_KEY_CHANNEL_2			P9
#define  SET_KEY_CHANNEL_3			P3

#define  SET_PWM_CHANNEL_1			P3
#define  SET_PWM_CHANNEL_2			P7
#define  SET_PWM_CHANNEL_3			P20
#define  SET_PWM_CHANNEL_4			P18
	
#define  SET_GPIO_OUTPUT_2			P15   //mic电源控制

#define  SET_ADC_CHANNEL_1			P14   //mic信号

#define  SET_ENABLE					0
#define  SET_TEST_GPIO				P20   //TEST 
	
#define  GPIO_UART_TX_NFC			P2
#define  GPIO_UART_RX_NFC			P34
	
#define  GPIO_433_REVEIVE			P14

#define MY_GPIO_DEP_SPICLK			0xff
#define MY_GPIO_DEP_SPICS			0xff
#define MY_GPIO_DEP_SPIDO			P34     //spi通讯控制引脚
#endif

#define  SET_PWM_CNTTOP_VALUE		160     //PWM 级数
#define  UI_100MS_EVENT_TICK		0x0008     //ui事件

#define  UI_BLE_TIMEOUT_TICK_1		0x0010
#define  UI_BLE_TIMEOUT_TICK_2		0x0020
#define  UI_BLE_TIMEOUT_TICK_3		0x0040
#define  EVENT_Sleep				0x0080
  
#define  KEY_EVENT_TICK_1			0x0100
#define  LED_EVENT_TICK				0x0200
#define  ADC_1_EVENT_TICK			0x0400     //adc事件的一级响应
#define  ADC_STOP_EVENT_TICK		0x1000     //adc事件的一级响应
#define  HALFS_EVENT_TICK			0x0800	
typedef struct {
	u8 device_onoff_flag;			//设备开关标志位
	u8 R_Work_Mold;					//工作模式
	u8 R_Work_Level;				//工作档位
	u8 R_DisPlay_Time;				//按键显示时间
	u8 Music_Number;				//语音脉冲个数（N个100US）
	u16 NTC1_Value;
	u16 NTC2_Value;
	
	u8 device_time_on[5];			//当前定时开设置状态  0:当前定时开关  1:工作星期  2:小时 3:分钟
	u8 device_time_off[5];			//当前定时关设置状态
	u8 get_adc_enable;				//避开DMA中断
  
	u8 dev_app_push_data[40] ;		//app下发数据缓存
	u8 key_dowm_tick;				//按键按下标志位
	u32 key_dowm_time ;				//按键按下是的系统time
	u8 R_Cool_Delay;
	bool B_Cool_Delay;
	bool B_NTC_Err;
	bool B_NTC_CoolErr;
	bool B_NTC_HotErr;
	
}struct_device_all ;



typedef struct {
	u32  LiEKValue[3];
	u8   uEKFlag[3]; 
	u8   uKP_Coe;
	u8   uKI_Coe;
	u8   uKD_Coe;
	u8   iPriValue;
	u8   iSetValue;
	u8   iCurValue;

}struct_PID_ALL ;

extern struct_device_all device_all_data ;   //当前设备的全部状态标志位


extern bool B_500ms_Flag;
extern u8 LED_NUM[3];

extern void lxl_SimpleProfile_SetParameter(uint8 param, uint8 len, void *value );  //BLE -> APP
//------------------中断函数----------------------//
extern void pin_event_handler_key(GPIO_Pin_e pin,IO_Wakeup_Pol_e type);

//------------------other-------------------------//
extern void HW_timer_100us(void);     //定时器设置
extern void HW_time_100us_stop(void);      //关断定时器
extern void proc_powerOff_handle(void);
extern void l_judge_app_data(u8 app_data[]);
extern void l_judge_time(void);   //判断定时开关

extern void send_type_request(u8 send_type);
extern void command_send_app_data(void);

//------------------事件函数----------------------//
extern void APP_init_task_func(u8 task_id);
extern u16 APP_all_function(u8 task_id,u16 events);


extern u8 APP_TaskID;

extern void uart_printf_hex (const uint8 *data, uint16 len);
extern u32 clock_time_exceed_func(u32 ref, u32 span_ms);
//------------------ADC 调用--------------------//
extern int normal_adc_init(void);
//------------------PWM 调用--------------------//
extern void Fan_pwm_timeout_handle(void);
extern void Fan_pwm_ctrl(u8 Fan_PWM);
extern void Fan_off(void);
extern void F_DisPlay(void);
//------------------RTC定时函数定义-------------//
#define LXL_2000_UNIX_TIME     0

typedef struct {
	u32 dev_time_at ;  //当前时间戳

	u32 dev_year ;     //当前时间"年"
	u32 dev_month ;    //当前时间"月"
	u32 dev_day ;      //当前时间"日"
	u32 dev_week ;     //当前时间"周"
				
	u32 dev_hour ;     //当前时间"时"
	u32 dev_minute ;   //当前时间"分"
	u32 dev_second ;   //当前时间"秒"
}lxl_time_rtc_all ;
extern lxl_time_rtc_all lxl_time_rtc_data ;  //当前rtc的定时器数据

typedef struct
{
	u8 Alarm_enable ;    //闹钟使能
	u8 Alarm_year ;      //闹钟 年
	u8 Alarm_month ;     //闹钟 月
	u8 Alarm_day ;       //闹钟 日
	u8 Alarm_week ;      //闹钟 周
	u8 Alarm_hour ;      //闹钟 时
	u8 Alarm_minute ;    //闹钟 分
	u8 Alarm_second ;    //闹钟 秒
	u8 Alarm_type ;      //闹钟  类型
}RTC8563_Alarm_Data;//RTC8563时间结构体
extern RTC8563_Alarm_Data my_alarm_data_1[7] ;   //闹钟结构体


typedef struct
{
	u8 IRQ_Data;
	u8 BatErr_Data;
	u8 SysErr_Data;
	u8 QCDirect_Data;
	u8 Status_Data;
	u8 BatTure_Data;	
}SW6206_REG_Data;
extern SW6206_REG_Data SW6206_all_Data;

extern u8 R_Step_Cnt;
extern u16 R_Time_Cnt;
extern bool B_Start_Time;

extern u8 lxl_last_button_numbale;
extern void lxl_set_rtc_clock_data_func(u32 time_clock);   //设置系统时间
extern void lxl_get_rtc_time_data_func(void);    //获取系统时间 
extern u8 my_rtc_week_count(u32 Year, u8 Month, u8 Day) ;  //《当前年月日算出周几》(Year_h：年高位（20）, Year_l:年低位(year - 2000)
extern void my_rtc_localtime(u32 time,lxl_time_rtc_all *t) ;  //《根据U32秒算出当前年月日》(time:U32 秒, time_count_temp *t:年月日)
extern int my_rtc_isleap(int year) ; //《校验是否为闰年》
extern u32 my_rtc_mktime(lxl_time_rtc_all dt) ;  //《根据当前年月日算出U32秒》
extern u32 my_10_switch_16_func(u32 data) ;  // 10进制转换成16进制
extern u32 my_16_switch_10_func(u32 data) ;  // 16进制转换成10进制
//------------------RGB ------------------------//

void gpio_wakeup_handle(void);
void gpio_sleep_handle(void);
extern void all_gpio_input(void);
extern void GPIO_A_High(void);
extern void GPIO_A_Low(void);
extern void GPIO_B_High(void);
extern void GPIO_B_Low(void);
extern void GPIO_C_High(void);
extern void GPIO_C_Low(void);
extern void GPIO_D_High(void);
extern void GPIO_D_Low(void);
extern void GPIO_E_High(void);
extern void GPIO_E_Low(void);
extern void Fan_Open(void);
extern void Fan_Close(void);
extern void Hot_Open(void);
extern void Cool_Open(void);
extern void Hot_Cool_Close(void);
extern void IIC_init(void);
extern void SW6206_DisCharge_Contorl(void);
extern void SW6206_Charge_Contorl(void);
extern void F_Work_Function(void);
extern void Hot_Time_Contor(void);
extern void NTC_Check(void);
extern void my_i2c_sim_burst_write(u8 id, u8 addr,u8 data); 
extern void hal_low_power_io_init(void);
extern void IO_Init(void);
//===============================END==========================//	
#ifdef __cplusplus
}
#endif


#endif
