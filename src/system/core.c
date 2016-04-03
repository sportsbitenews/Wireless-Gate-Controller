/*
 * core.c
 *
 *  Created on: 27 Mar 2016
 *      Author: Daniel
 */

#include "core.h"

extern volatile uint8_t timer1_interrupt;

//this variable gets modified every 1ms by the timer interrupt
extern volatile uint8_t timer2_interrupt;

extern volatile uint8_t rx_flag;

void core_setup(){
	ADC_done = 0;
	INTAD_FSM_state = AD_IDLE;

	debug_adc_serial = 0;

	nSLEEP = 0;

	ir_reset_counters();

	R_TAU0_Channel0_Start();
	R_TAU0_Channel1_Lower8bits_Start();
	R_TAU0_Channel2_Start();
	R_TAU0_Channel3_Start();

	R_UART1_Start();
	delay(100);
	//	R_INTC3_Start();

	R_TMR_RD0_Start();

	initLcd();

	print_long_message("Robinson DL 18361137");
	delay(100);

	R_UART1_Receive(&serial_rx,1);	// Prime UART1 Rx

	echo(0x80);
	print_lcd("normal_mode");
	/* Peripheral start function calls */
	R_ADC_Set_OperationOn(); /* Enable ADC voltage comparator */

	motor_power(28);
}

void core_main(){

	while (1U){
		if (rx_flag){
			rx_flag = 0;
			serial_handler();
		}

		if (timer2_interrupt){
			timer2_interrupt = 0;
			gate_stop_handler();
		}

		if (timer1_interrupt){
			timer1_interrupt = 0;
			adc_get_multiple_channels();
		}
	}
}
