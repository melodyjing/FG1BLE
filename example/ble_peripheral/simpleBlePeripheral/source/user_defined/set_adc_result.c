/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：set_adc_result.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-5-31
	Author:   melody
	Description:This file contains set adc result. 
************************************************************************/
#include "all_gpio_init.h"

//---------------ADC通道选择------------------//
adc_Cfg_t normal_adc_cfg = 
{
	//.channel = ADC_BIT(ADC_CH1N_P11)|ADC_BIT(ADC_CH1P_P12)|ADC_BIT(ADC_CH2N_P13)|ADC_BIT(ADC_CH2P_P14)|ADC_BIT(ADC_CH3N_P15),
	.channel = ADC_BIT(ADC_CH3N_P15)|ADC_BIT(ADC_CH2N_P24),			//配置ADC通道
	.is_continue_mode = FALSE,					//是否连续模式
	.is_differential_mode = 0x00,				//00单端模式，FF差分模式
	.is_high_resolution = 0x00,					//FF为引脚电压直接进入ADC（Vin<0.8），00为经ADC内部分压后1/4电压输入给ADC，（Vin<3.2V）。
};


//--------------结果返回---------------------//
void normal_adc_handle_evt(adc_Evt_t* pev)
{
	bool is_high_resolution = FALSE;
	bool is_differential_mode = FALSE;

	if(pev->type == HAL_ADC_EVT_DATA)
	{
		if(pev->ch==ADC_CH3N_P15)
		{
			is_high_resolution = (normal_adc_cfg.is_high_resolution & ADC_BIT(ADC_CH3N_P15))?TRUE:FALSE;
			is_differential_mode = (normal_adc_cfg.is_differential_mode & ADC_BIT(ADC_CH3N_P15))?TRUE:FALSE;
			device_all_data.NTC1_Value = hal_adc_value_cal((adc_CH_t)(ADC_CH3N_P15),pev->data, pev->size, is_high_resolution,is_differential_mode);
		}
		else if(pev->ch==ADC_CH2N_P24)
		{
			is_high_resolution = (normal_adc_cfg.is_high_resolution & ADC_BIT(ADC_CH2N_P24))?TRUE:FALSE;
			is_differential_mode = (normal_adc_cfg.is_differential_mode & ADC_BIT(ADC_CH2N_P24))?TRUE:FALSE;
			device_all_data.NTC2_Value = hal_adc_value_cal((adc_CH_t)(ADC_CH2N_P24),pev->data, pev->size, is_high_resolution,is_differential_mode);
		}
	}
}

int normal_adc_init(void)
{
	int ret;  
	ret = hal_adc_config_channel(normal_adc_cfg, normal_adc_handle_evt);		//初始化ADC口配置
	if(ret)
	return ret;
	return PPlus_SUCCESS;
}

//----------------------ADC------------------------//
/*
const uint32_t adc_base_addr[6] = 
{
	0x40050500,
	0x40050580,
	0x40050600,
	0x40050680,
	0x40050700,
	0x40050780 
};


float my_hal_adc_value_cal(adc_CH_t ch,uint16_t buf,uint8_t high_resol, uint8_t diff_mode)
{
    volatile float result = 0.0;
    result = buf;
	result = (diff_mode) ? (float)(result / 2048 -1) : (float)(result /4096);
    if(high_resol == TRUE)
    {
        result *= 0.8;
    }
    return result;
}

uint16_t adc_buf[64];
uint16_t adc_sample_buf[58];

uint16 adcMeasureCh(ADC_CH_R_e adc_ch)
{
	uint32_t adc_sample_data[32];
	adc_init_r(adc_ch, SINGLE_END, BELOW_8000MV); 		//adc init
	subWriteReg(0x4000f07c, 2, 1, 2);  					//0：80K   1:160K    2:320k rate sample			
	WaitUs(300);										/// 320K  64dot		wait 
	for (uint8_t i = 0; i < 29; i++)
	{
		adc_sample_data[i] = read_reg(adc_base_addr[adc_ch-2] + (i << 2)); // 0x0700~0x077F offset=0x4
		adc_buf[i*2] = adc_sample_data[i]&0x0fff;
		adc_buf[i*2+1] = (adc_sample_data[i]>>16)&0x0fff;
	}
	adc_stop_r();	
//	InsertSort(adc_buf, 58);		/// 排序
	uint32 sum_adc = 0;
	for(uint8 i=24;i<40;i++) 	///取中间16个数据的均值
	{				
		sum_adc+=adc_buf[i];
	}			
	sum_adc = (sum_adc>>4);	

	return sum_adc;
}*/
