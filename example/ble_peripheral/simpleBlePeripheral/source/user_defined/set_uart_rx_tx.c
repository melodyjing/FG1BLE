#include "all_gpio_init.h"

struct_uart_all uart_all_data = {
	.push_data_numable=0,          //上传数据的大小
	.push_tick=0,                  //开始上传数据标志位
	.tx_cnt=0,                     //tx发送数据个数
	.tx_buf={0},                   //tx发送数据
	.rx_cnt=0,                     //rx接收数据个数
	.rx_buf={0},                   //rx接收数据
};    //当前sensor的数据
void uart_console_rx_handler(uart_Evt_t* pev)
{
	struct_uart_all* pctx = &uart_all_data;
	switch(pev->type){
		case UART_EVT_TYPE_RX_DATA:      //rx获取数据中
		case UART_EVT_TYPE_RX_DATA_TO:   //rx获取数据超时
			if((pctx->rx_cnt+pev->len)>=255){
				break ;
			}
			if(uart_all_data.push_tick == 1)
				break ;
			osal_memcpy(pctx->rx_buf+pctx->rx_cnt,pev->data,pev->len) ;
			pctx->rx_cnt += pev->len ;
	
			//hal_uart_send_buff(pctx->rx_buf,pctx->rx_cnt) ;
			osal_stop_timerEx(uart_count_TaskID, UART_1_EVENT_TICK);
			osal_start_timerEx(uart_count_TaskID, UART_1_EVENT_TICK,10);
		break;
		default:
			break;
	}
}

void uart_sensor_gpio_init(u8 mode)
{
	uart_Cfg_t cfg2 = {
		.tx_pin = GPIO_UART_TX_NFC,
		.rx_pin = GPIO_UART_RX_NFC,
		.rts_pin = GPIO_DUMMY,
		.cts_pin = GPIO_DUMMY,
		.baudrate = 115200,
		.use_fifo = TRUE,
		.hw_fwctrl = FALSE,
		.use_tx_buf = FALSE,
		.parity     = FALSE,
		.evt_handler = uart_console_rx_handler,
	};
	hal_uart_init(cfg2,UART1);//uart init
	LOG("uart_2222222222\n");
}

void turn_off_uart(void)  //关闭串口
{
	hal_uart_deinit(UART1);
  hal_gpio_pin_init(GPIO_UART_TX_NFC,IE) ;                  //设置为输入
  hal_gpio_pull_set(GPIO_UART_RX_NFC,FLOATING) ;      //设置当前io的上下拉	
}



