/*
 * event_groups.c
 *
 * Created: 5/26/2022 9:14:43 AM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

#include <stdio_driver.h>
#include <serial.h>

#define BIT_MEASURE_CO2	( 1 << 0 ) // BIT 0
#define BIT_MEASURE_HUM_TEMP ( 1 << 1 ) // BIT 1
#define BIT_MEASURE_LUX ( 1 << 2 ) // BIT 2
#define BIT_MEASURE_ALL (BIT_MEASURE_CO2 | BIT_MEASURE_HUM_TEMP | BIT_MEASURE_LUX)

#define BIT_READY_CO2 ( 1 << 0 ) // BIT 0
#define BIT_READY_HUM_TEMP ( 1 << 1 ) // BIT 1
#define BIT_READY_LUX ( 1 << 2 ) // BIT 2
#define BIT_READY_ALL (BIT_READY_CO2 | BIT_READY_HUM_TEMP | BIT_READY_LUX)

EventGroupHandle_t dataReadyEventGroup;
EventGroupHandle_t measureEventGroup;
EventGroupHandle_t numbersEventGroup;

EventGroupHandle_t getMeasureEventGroup(){
	return measureEventGroup;
}

EventGroupHandle_t getNumbersEventGroup(){
	return numbersEventGroup;
}

EventGroupHandle_t getDataReadyEventGroup(){
	return dataReadyEventGroup;
}

void initEventGroups(){
    
    /* Attempt to create the event group. */
    measureEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
    if( measureEventGroup == NULL )
    {
		printf("Not enough memory.");
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
    }
    else
    {
		printf("Measure group good\n");
        /* The event group was created. */
    }

	/* Attempt to create the event group. */
    numbersEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
    if( numbersEventGroup == NULL )
    {
		printf("Not enough memory.");
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
    }
    else
    {
		printf("Num gr OK\n");
        /* The event group was created. */
    }

    /* Attempt to create the event group. */
    dataReadyEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
    if( dataReadyEventGroup == NULL )
    {
		printf("Not enough memory.");
		
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
    }
    else
    {
		printf("Data Ready group good\n");
        /* The event group was created. */
    }
}