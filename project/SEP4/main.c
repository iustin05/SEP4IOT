/*
 * main.c
 *
 * Created: 5/26/2022 12:44:07 AM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

#include <util/delay.h>

#include <app_tasks.h>

#include <status_leds.h>

#include <init_sensors.h>
#include <rc_servo.h>

void createTasks()
{
	xTaskCreate(
	appTask
	,  "MainAppTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+150  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	CO2Task
	,  "CO2Task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+100  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	tempHumTask
	,  "TempHumTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+100  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	luxTask
	,  "LuxTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+100  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	servoTask
	,  "ServoTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+100  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	upLinkTask
	,  "UpLinkTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+100  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	downLinkTask
	,  "DownLinkTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+100  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
}

int main(void){
	stdio_initialise(ser_USART0);
	puts("--------------------BOOTING-------------------");
	_delay_ms(1000);
	puts("main() started, communication to serial set\n");
	puts("---------------------------------------------");
	createTasks();
	puts("Tasks created\n");
	puts("---------------------------------------------");
	initSensorsAndEvents();
	vTaskStartScheduler();
	
	while(1){
		
	}
}