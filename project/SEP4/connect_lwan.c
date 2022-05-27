/*
 * connect_lwan.c
 *
 * Created: 5/27/2022 2:25:00 PM
 *  Author: nordesk
 */ 

#include <stdio.h>
#include <avr/io.h>

#include <stdio_driver.h>
#include <serial.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>

#include <lora_driver.h>
#include <lora_driver_utils.h>

#include <status_leds.h>

#define LORA_appEUI "2BBE8F09765BBF4B"
#define LORA_appKEY "5F83717BC67B4646E3F00DC5EC3417DC"

void initLORAWAN() {
	char hweui[20];
	
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	vTaskDelay(150);
	lora_driver_flushBuffers();
	
	
	lora_driver_rn2483FactoryReset();
	lora_driver_configureToEu868();
	lora_driver_getRn2483Hweui(hweui);
	lora_driver_setDeviceIdentifier(hweui);
	lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,hweui);
	lora_driver_saveMac();
	lora_driver_setAdaptiveDataRate(LORA_ON);
	lora_driver_setReceiveDelay(500);
	uint8_t maxJoinTriesLeft = 10;
	lora_driver_returnCode_t rc;
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Nwrk Try Left: %d > %s <\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);
	
	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST3); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}