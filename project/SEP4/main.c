/*
 * main.c
 *
 * IMPORTANT!
 * To configure the program, visit "lib/program_config.h"
 *
 * Created: 5/26/2022 12:44:07 AM
 *  Author: nordesk
 */ 
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>

#include <serial.h>

#include <app_tasks.h>

#include <program_config.h>

#include <status_leds.h>

#include <xevent_groups.h>

#include <sensors_callback.h>

#include <comm_queue.h>

#include <message_buffers.h>

#include <lora_driver.h>

#include <display_7seg.h>

#include <leds_numbers_tasks.h>

void createTasks()
{
	xTaskCreate(
	appTask
	,  "MainAppTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+150  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	CO2Task
	,  "CO2Task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	tempHumTask
	,  "TempHumTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	luxTask
	,  "LuxTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	servoTask
	,  "ServoTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	upLinkTask
	,  "UpLinkTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	downLinkTask
	,  "DownLinkTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	ledsAndNumbersTask
	,  "LEDsAndNumbers"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
}

void initLEDsAndDisplay()
{
	stdio_initialise(ser_USART0);
	DDRA |= _BV(DDA1) | _BV(DDA2) | _BV(DDA3) | _BV(DDA4) | _BV(DDA5) | _BV(DDA6) | _BV(DDA7) | _BV(DDA0);
	status_leds_initialise(5);
	display_7seg_initialise(NULL);
	display_7seg_powerUp();
}

void initMain()
{
	initLEDsAndDisplay();
	initConfiguration();
	initServo();
	initEventGroups();
	initTempHum();
	initMessageBuffers();
	setLuxCallback();
	setCO2Callback();
	createTasks();
	createQueue();
	#ifndef DEBUG_NONETWORK
	lora_driver_initialise(1, getDownLinkMessageBuffer());
	printf("Lora OK\n");
	#endif
}

int main(void){
	initMain();
	vTaskStartScheduler();
	
	while(1){
		printf("Error");
	}
}