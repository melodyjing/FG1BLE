/***********************************************************************
    Copyright (C) 2020-2022,lanhe-tech software development team
 	Filename：other_function.c
	Revised:  v1.0       
	Revision:
	Date:	  2022-06-02
	Author:   melody
	Description:This file contains for i2c driver. 
************************************************************************/
#include "all_gpio_init.h"

 SW6206_REG_Data SW6206_all_Data = 
{
	.IRQ_Data = 0,
	.BatErr_Data =0,
	.SysErr_Data=0,
	.QCDirect_Data=0,
	.Status_Data=0,
	.BatTure_Data=0,		
};
 
void IIC_init(void)    
{	
	hal_gpio_pin_init(SW6206_SDA, IE) ;                  //输入
	hal_gpio_pull_set(SW6206_SDA, STRONG_PULL_UP) ;      //上拉
	
	hal_gpio_pin_init(SW6206_SCL, IE) ;                  //输入
	hal_gpio_pull_set(SW6206_SCL, STRONG_PULL_UP) ;      //上拉
}
static inline void my_i2c_sim_wait(void)
{
	;;;;;;;;;;;;;;;;;;;;;
}
void my_i2c_sim_long_wait(void)
{
	WaitUs(10);
}
static inline void my_i2c_sim_scl_init(void)		//初始化位GPIO
{    
	hal_gpio_fmux(SW6206_SCL,Bit_DISABLE) ;              
}

static inline void my_i2c_sim_scl_idle(void)		//输入高电平
{   
	hal_gpio_pin_init(SW6206_SCL,OEN) ;               
	hal_gpio_write(SW6206_SCL,0) ;                      
}

static inline void my_i2c_sim_scl_out(int v)		//输入
{   
	if(v)
	{
		hal_gpio_pin_init(SW6206_SCL,IE) ;                
	}
	else 
	{
		hal_gpio_pin_init(SW6206_SCL,OEN) ;                  
	}
}
static inline void my_i2c_sim_sda_init(void)
{
	hal_gpio_fmux(SW6206_SDA,Bit_DISABLE) ;   
    hal_gpio_pin_init(SW6206_SDA,IE) ;                 
}
static inline void my_i2c_sim_sda_idle(void)
{
	hal_gpio_pin_init(SW6206_SDA,OEN) ;                 
	hal_gpio_write(SW6206_SDA,0) ;                       
}
static inline void my_i2c_sim_sda_out(int v)
{ 
	if(v)
	{
		hal_gpio_pin_init(SW6206_SDA,IE) ;                 
	}
	else 
	{
		hal_gpio_pin_init(SW6206_SDA,OEN) ;               
	}
}
static inline int my_i2c_sim_sda_in(void)
{    
	return hal_gpio_read(SW6206_SDA);
}
void my_i2c_sim_start(void)
{
	my_i2c_sim_scl_init();     //init
	my_i2c_sim_sda_init();     //init
	my_i2c_sim_sda_idle();     //sda: 1
	my_i2c_sim_scl_idle();     //scl: 1
	my_i2c_sim_sda_out(0);		//sda: 0
	my_i2c_sim_wait();         //daley
}

void my_i2c_sim_stop(void)
{
	my_i2c_sim_sda_out(0);   //sda: 0
	my_i2c_sim_wait();
	my_i2c_sim_scl_out(0);   //scl: 0
	my_i2c_sim_wait();
	my_i2c_sim_scl_out(1);   //scl: 1
	my_i2c_sim_wait();
	my_i2c_sim_sda_out(1);   //sda: 1
}

u8 my_i2c_wait_ack()   //等待ACK 应答
{
	u8 temp = 0 ;
	my_i2c_sim_sda_out(1);   //sda: 1
	my_i2c_sim_wait();
	my_i2c_sim_scl_out(1);   //scl: 1
	my_i2c_sim_wait();
	my_i2c_sim_sda_in() ;
	my_i2c_sim_wait();

	while(my_i2c_sim_sda_in())
	{
		temp++ ;
		if(temp>254)
		{
			my_i2c_sim_stop();
			return 1 ;
		}
	}
	my_i2c_sim_scl_out(0);   //scl: 0
	return 0 ;
}
void my_i2c_wait_Nack(void)  
{
	my_i2c_sim_scl_out(0);   //scl: 0
	my_i2c_sim_wait();
	my_i2c_sim_sda_out(1);   //sda: 1
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();

	my_i2c_sim_scl_out(1);   //scl: 1
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();
	my_i2c_sim_long_wait();
	my_i2c_sim_scl_out(0);   //scl: 0
}


static void my_i2c_sim_wirte_bit(int bit)
{
	my_i2c_sim_scl_out(0);
	my_i2c_sim_sda_out(bit);
	my_i2c_sim_long_wait();
	my_i2c_sim_scl_out(1);
//	my_i2c_sim_long_wait();
}

static int my_i2c_sim_read_bit(void)
{
	my_i2c_sim_wirte_bit(1);
	return my_i2c_sim_sda_in();
}

int my_i2c_sim_write_byte(u8 dat)
{
	int i = 0x80;
	while(i)
	{
		my_i2c_sim_wirte_bit((dat & i));
		i = i >> 1;
	}
	return my_i2c_sim_read_bit();
}

u8 my_i2c_sim_read_byte(int last)
{
	u8 dat = 0;
	for(u8 i=0;i<8; i++)
	{
		my_i2c_sim_wirte_bit(1);
		if(my_i2c_sim_sda_in())
		{
			dat =(dat << 1) | 0x01;
		}
		else
		{
			dat = dat << 1;
		}
	}
	my_i2c_sim_wirte_bit(last);
	return dat;
}

void my_i2c_sim_write(u8 id, u8 addr, u8 dat)
{
	my_i2c_sim_start();
	my_i2c_sim_write_byte(id);
	my_i2c_sim_write_byte(addr);
	my_i2c_sim_write_byte(dat);
	my_i2c_sim_stop();
}

//写数据  id: 从机地址  addr:寄存器首弟子  *P:数据   n:字节长度
void my_i2c_sim_burst_read(u8 id, u8 addr,u8 *p, int n)
{
	my_i2c_sim_start();

	my_i2c_sim_write_byte(id);
	my_i2c_sim_write_byte(addr);
	my_i2c_sim_stop();
	my_i2c_sim_start();

	my_i2c_sim_write_byte(id | 1);

	for(int k = 0; k < n; ++k)
	{
		*p++ = my_i2c_sim_read_byte( k ==(n-1) );
	}
	my_i2c_sim_stop();
}

u8 my_i2c_sim_read(u8 id, u8 addr)
{
	u8 dat;
	my_i2c_sim_burst_read(id, addr, &dat, 1);
	return dat;
}
//写数据  id: 从机地址  addr:寄存器首弟子  *P:数据   n:字节长度
void my_i2c_sim_burst_write(u8 id, u8 addr,u8 data)   
{
	my_i2c_sim_start();
	my_i2c_sim_write_byte(id);
	my_i2c_sim_write_byte(addr);
	my_i2c_sim_write_byte(data);
	my_i2c_sim_stop();
}

void SW6206_Charge_Contorl(void)
{
	SW6206_all_Data.IRQ_Data=my_i2c_sim_read(SW6206_Device,IRQ_REG);						//06
	SW6206_all_Data.BatErr_Data = my_i2c_sim_read(SW6206_Device,BatErr_REG);				//0A	
	SW6206_all_Data.SysErr_Data=my_i2c_sim_read(SW6206_Device,SysErr_REG);					//0B
	SW6206_all_Data.Status_Data=my_i2c_sim_read(SW6206_Device,Status_REG);					//0c
	SW6206_all_Data.QCDirect_Data=my_i2c_sim_read(SW6206_Device,QCDirect_REG);				//1F		Bit3为1快充
	SW6206_all_Data.BatTure_Data=my_i2c_sim_read(SW6206_Device,BatTure_REG);				//7E

}				

void SW6206_DisCharge_Contorl(void)
{
	SW6206_all_Data.IRQ_Data=my_i2c_sim_read(SW6206_Device,IRQ_REG);						//06
//	if((SW6206_all_Data.IRQ_Data&0x04)==0)													
	{
		my_i2c_sim_burst_write(SW6206_Device,0x04,0x01);
	}
	SW6206_all_Data.BatErr_Data = my_i2c_sim_read(SW6206_Device,BatErr_REG);				//0A	
	SW6206_all_Data.SysErr_Data=my_i2c_sim_read(SW6206_Device,SysErr_REG);					//0B
	SW6206_all_Data.Status_Data=my_i2c_sim_read(SW6206_Device,Status_REG);					//0c
	SW6206_all_Data.QCDirect_Data=my_i2c_sim_read(SW6206_Device,QCDirect_REG);				//1F		Bit3为1快充
	SW6206_all_Data.BatTure_Data=my_i2c_sim_read(SW6206_Device,BatTure_REG);				//7E


}

