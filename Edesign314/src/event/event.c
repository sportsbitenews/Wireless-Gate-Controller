/*
 * event.c
 *
 *  Created on: 02 May 2016
 *      Author: Daniel
 */

#include "event.h"

void eventAdd(datalog_t event){
	static uint8_t constructor = 1;
	if (constructor){
		event_index = -1;
		constructor--;
	}
	event_index++;
	event_index%=max_events;
	rtc_counter_value_t time_now;
	R_RTC_Get_CounterValue(&time_now);
	event.time = time_now;
	event_datalogs[event_index] = event;
}

void eventPrint(datalog_t event){
	uint8_t lcd_event[16];
	//	lcd_event[0] = (event.time.month >> 4) + '0';
	lcd_event[0] = (event.time.month & 0x0F) + '0';
	//	lcd_event[2] = (event.time.day >> 4) + '0';
	lcd_event[1] = (event.time.day & 0x0F) + '0';
	lcd_event[2] = (event.time.hour >> 4) + '0';
	lcd_event[3] = (event.time.hour & 0x0F) + '0';
	lcd_event[4] = (event.time.min >> 4) + '0';
	lcd_event[5] = (event.time.min & 0x0F) + '0';
	lcd_event[6] = (event.time.sec >> 4) + '0';
	lcd_event[7] = (event.time.sec & 0x0F) + '0';

	uint8_t * lcd_event_string = " unknown";
	switch (event.event){
	case event_open:
		lcd_event_string = "    open";
		break;
	case event_close:
		lcd_event_string = "   close";
		break;
	case event_opened:
		lcd_event_string = "  opened";
		break;
	case event_closed:
		lcd_event_string = "  closed";
		break;
	case event_emergency_stopped:
		lcd_event_string = "Estopped";
		break;
	case event_autoclose:
		lcd_event_string = "autclose";
		break;
	case event_autoclosed:
		lcd_event_string = "auclosed";
		break;
	case event_mech_interference:
		lcd_event_string = "  object";
		break;
	}

	uint8_t i;
	for (i = 8; i < 16; i++){
		lcd_event[i] = lcd_event_string[i - 8];
	}

	print_lcd(lcd_event);
}
