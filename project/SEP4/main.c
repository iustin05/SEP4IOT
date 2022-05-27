/*
 * main.c
 *
 * Created: 5/26/2022 12:44:07 AM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

#include <app_tasks.h>

#include <status_leds.h>

#include <xevent_groups.h>

#include <sensors_callback.h>

#include <comm_queue.h>

#include <message_buffer.h>
#include <message_buffers.h>

#include <lora_driver.h>

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
	
}

void initMain()
{
	stdio_initialise(ser_USART0);
	DDRA |= _BV(DDA0) | _BV(DDA7);
	status_leds_initialise(5);
	initConfiguration();
	initServo();
	initEventGroups();
	initTempHum();
	initMessageBuffers();
	setLuxCallback();
	setCO2Callback();
	createTasks();
	createQueue();
	lora_driver_initialise(1, getDownLinkMessageBuffer());
	printf("LoraWAN OK\n");
}

int main(void){
	initMain();
	printf("BOOT OK\n");
	vTaskStartScheduler();
	
	while(1){
		printf("Error");
	}
}