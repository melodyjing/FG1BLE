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

#define	 Cool_Contorl_A				P7			//����ƬA��
#define	 Cool_Contorl_B				P31			//����ƬB��

#define	 SW6206_IRQ					P11
#define	 SW6206_SCL					P2
#define	 SW6206_SDA					P3


#define	 Fan_GPIO					P18

#define	 Speech_Contorl				P20

#define	 NTC1_CHANNEL				P24
#define	 NTC2_CHANNEL				P15



#define SW6206_Device			    0x78

#define KeyConf_REG					0x03	//��������
#define ShortKey_REG				0x04	//�̰��¼�
#define	IRQ_REG						0x06	//����״̬	
#define	Pullout_REG					0x08	//�˿ڰγ�
#define	Pullin_REG					0x09	//�˿ڲ���
#define	BatErr_REG					0x0A	//����쳣
#define	SysErr_REG					0x0B	//ϵͳ�쳣
#define Status_REG					0x0C	//ϵͳ״̬
#define PDContorl_REG				0x0F	//���Э��
#define	ADCConf_REG					0x12	//ADC����
#define ADCH_REG					0x13	//ADC�߰�λ
#define ADCL_REG					0x14	//ADC����λ
#define OutInControl_REG			0x18	//�����������ʹ��
#define PortEvent_REG				0x19	//�˿��¼�����
#define QCConfig0_REG				0x1A	//�������0
#define QCConfig1_REG				0x1B	//�������1
#define QCConfig2_REG				0x1C	//�������2
#define QCConfig3_REG				0x1D	//�������3
#define QCConfig4_REG				0x1E	//�������4  BIT4:0֧�ֱ߳�߷ţ�1��֧�ֱ߳�߷ţ�������ȣ�BIT1,C��QC sourceʹ�� 0ʹ��
#define QCDirect_REG				0x1F	//���ָʾ
#define OVP_REG						0x21	//VDD��ѹ�¼���BIT5
#define CMD_REG						0x22	//PD��� BIT3-0:1Ϊ��������
#define TypeCconfig_REG				0x28	//TypeC���� BIT3-2:0 Strong drp,1 sink 2 source
#define TypeC_REG					0x29	//TypeCָʾ BIT1-0: 1sink 2source
#define PDConfig0_REG				0x2A	//PD����0
#define PDConfig1_REG				0x2B	//PD����1
#define PDConfig2_REG				0x2C	//PD����2
#define PD_REG						0x2D	//�ೡ��PD���� BIT5:0 �㲥5VPDO,1��ʹ�ܣ�BIT2����ѹSCP 0ʹ�� BIT0������1.2Vʹ�� 0ʹ��
#define SCurr_REG					0x2E	//С���������� BIT4: 1������˳�С������磬BIT0:1����С�������״̬
#define Check0_REG					0x30	//����γ��������0
#define Check1_REG					0x31	//����γ��������1
#define Wireless_REG				0x32	//���߳�����
#define SCurrOut_REG				0x33	//С�����������
#define Boost0_REG					0x40	//Boost0����
#define Boost1_REG					0x41	//Boost1����
#define Chager0_REG					0x42	//Chager0����
#define Chager1_REG					0x43	//Chager1����
#define Chager2_REG					0x44	//Chager2����
#define Chager3_REG					0x45	//Chager3����
#define Chager4_REG					0x46	//Chager4����
#define NTC0_REG					0x47	//NTC����0
#define NTC1_REG					0x48	//NTC����1
#define Temp_REG					0x49	//�¶�����
#define Bat_REG						0x72	//���յ���
#define BatCapL_REG					0x73	//���������8λ
#define BatCapH_REG					0x74	//���������4λ
#define Charge_REG					0x7A	//�����ƣ�BIT5��0����ֵ��1,������ֵ��0.1V��BIT4:��������������ʹ�ܣ�1ʹ�ܣ�ͨ��BIT3���õ�����1Ϊ1A��0Ϊ0.5A
#define BatTure_REG					0x7E	//���մ������

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
	
#define  SET_GPIO_OUTPUT_2			P15   //mic��Դ����

#define  SET_ADC_CHANNEL_1			P14   //mic�ź�

#define  SET_ENABLE					0
#define  SET_TEST_GPIO				P20   //TEST 
	
#define  GPIO_UART_TX_NFC			P2
#define  GPIO_UART_RX_NFC			P34
	
#define  GPIO_433_REVEIVE			P14

#define MY_GPIO_DEP_SPICLK			0xff
#define MY_GPIO_DEP_SPICS			0xff
#define MY_GPIO_DEP_SPIDO			P34     //spiͨѶ��������
#endif

#define  SET_PWM_CNTTOP_VALUE		160     //PWM ����
#define  UI_100MS_EVENT_TICK		0x0008     //ui�¼�

#define  UI_BLE_TIMEOUT_TICK_1		0x0010
#define  UI_BLE_TIMEOUT_TICK_2		0x0020
#define  UI_BLE_TIMEOUT_TICK_3		0x0040
#define  EVENT_Sleep				0x0080
  
#define  KEY_EVENT_TICK_1			0x0100
#define  LED_EVENT_TICK				0x0200
#define  ADC_1_EVENT_TICK			0x0400     //adc�¼���һ����Ӧ
#define  ADC_STOP_EVENT_TICK		0x1000     //adc�¼���һ����Ӧ
#define  HALFS_EVENT_TICK			0x0800	
typedef struct {
	u8 device_onoff_flag;			//�豸���ر�־λ
	u8 R_Work_Mold;					//����ģʽ
	u8 R_Work_Level;				//������λ
	u8 R_DisPlay_Time;				//������ʾʱ��
	u8 Music_Number;				//�������������N��100US��
	u16 NTC1_Value;
	u16 NTC2_Value;
	
	u8 device_time_on[5];			//��ǰ��ʱ������״̬  0:��ǰ��ʱ����  1:��������  2:Сʱ 3:����
	u8 device_time_off[5];			//��ǰ��ʱ������״̬
	u8 get_adc_enable;				//�ܿ�DMA�ж�
  
	u8 dev_app_push_data[40] ;		//app�·����ݻ���
	u8 key_dowm_tick;				//�������±�־λ
	u32 key_dowm_time ;				//���������ǵ�ϵͳtime
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

extern struct_device_all device_all_data ;   //��ǰ�豸��ȫ��״̬��־λ


extern bool B_500ms_Flag;
extern u8 LED_NUM[3];

extern void lxl_SimpleProfile_SetParameter(uint8 param, uint8 len, void *value );  //BLE -> APP
//------------------�жϺ���----------------------//
extern void pin_event_handler_key(GPIO_Pin_e pin,IO_Wakeup_Pol_e type);

//------------------other-------------------------//
extern void HW_timer_100us(void);     //��ʱ������
extern void HW_time_100us_stop(void);      //�ض϶�ʱ��
extern void proc_powerOff_handle(void);
extern void l_judge_app_data(u8 app_data[]);
extern void l_judge_time(void);   //�ж϶�ʱ����

extern void send_type_request(u8 send_type);
extern void command_send_app_data(void);

//------------------�¼�����----------------------//
extern void APP_init_task_func(u8 task_id);
extern u16 APP_all_function(u8 task_id,u16 events);


extern u8 APP_TaskID;

extern void uart_printf_hex (const uint8 *data, uint16 len);
extern u32 clock_time_exceed_func(u32 ref, u32 span_ms);
//------------------ADC ����--------------------//
extern int normal_adc_init(void);
//------------------PWM ����--------------------//
extern void Fan_pwm_timeout_handle(void);
extern void Fan_pwm_ctrl(u8 Fan_PWM);
extern void Fan_off(void);
extern void F_DisPlay(void);
//------------------RTC��ʱ��������-------------//
#define LXL_2000_UNIX_TIME     0

typedef struct {
	u32 dev_time_at ;  //��ǰʱ���

	u32 dev_year ;     //��ǰʱ��"��"
	u32 dev_month ;    //��ǰʱ��"��"
	u32 dev_day ;      //��ǰʱ��"��"
	u32 dev_week ;     //��ǰʱ��"��"
				
	u32 dev_hour ;     //��ǰʱ��"ʱ"
	u32 dev_minute ;   //��ǰʱ��"��"
	u32 dev_second ;   //��ǰʱ��"��"
}lxl_time_rtc_all ;
extern lxl_time_rtc_all lxl_time_rtc_data ;  //��ǰrtc�Ķ�ʱ������

typedef struct
{
	u8 Alarm_enable ;    //����ʹ��
	u8 Alarm_year ;      //���� ��
	u8 Alarm_month ;     //���� ��
	u8 Alarm_day ;       //���� ��
	u8 Alarm_week ;      //���� ��
	u8 Alarm_hour ;      //���� ʱ
	u8 Alarm_minute ;    //���� ��
	u8 Alarm_second ;    //���� ��
	u8 Alarm_type ;      //����  ����
}RTC8563_Alarm_Data;//RTC8563ʱ��ṹ��
extern RTC8563_Alarm_Data my_alarm_data_1[7] ;   //���ӽṹ��


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
extern void lxl_set_rtc_clock_data_func(u32 time_clock);   //����ϵͳʱ��
extern void lxl_get_rtc_time_data_func(void);    //��ȡϵͳʱ�� 
extern u8 my_rtc_week_count(u32 Year, u8 Month, u8 Day) ;  //����ǰ����������ܼ���(Year_h�����λ��20��, Year_l:���λ(year - 2000)
extern void my_rtc_localtime(u32 time,lxl_time_rtc_all *t) ;  //������U32�������ǰ�����ա�(time:U32 ��, time_count_temp *t:������)
extern int my_rtc_isleap(int year) ; //��У���Ƿ�Ϊ���꡷
extern u32 my_rtc_mktime(lxl_time_rtc_all dt) ;  //�����ݵ�ǰ���������U32�롷
extern u32 my_10_switch_16_func(u32 data) ;  // 10����ת����16����
extern u32 my_16_switch_10_func(u32 data) ;  // 16����ת����10����
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
