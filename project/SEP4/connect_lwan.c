/*
 * connect_lwan.c
 *
 * Created: 5/27/2022 2:25:00 PM
 *  Author: nordesk
 */ 

#include <iot_io.h>

#include <stdlib.h>

#include <ATMEGA_FreeRTOS.h>

#include <program_config.h>

#include <lora_driver.h>

#include <leds_numbers_tasks.h>

#include <display_7seg.h>

#include <status_leds.h>



void initLORAWAN() {
	#ifdef DEBUG_NONETWORK
	printf("NONETWORK\n");
	#endif // DEBUG_NONETWORK
	
	#ifndef DEBUG_NONETWORK
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
	lora_driver_setOtaaIdentity(SETTING_LORA_APPEUI,SETTING_LORA_APPKEY,hweui);
	lora_driver_saveMac();
	lora_driver_setAdaptiveDataRate(LORA_ON);
	lora_driver_setReceiveDelay(SETTING_LORA_RECEIVEDELAY);
	uint8_t maxJoinTriesLeft = SETTING_LORA_MAXJOINTRIES;
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST3);
	do {
		char* str = malloc( 2 );
		snprintf( str, 2, "%d", maxJoinTriesLeft);
		display_7seg_displayHex(str);
		free(str);
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
		display_7seg_displayHex("9000");
		status_leds_ledOn(led_ST2); // OPTIONAL
		status_leds_ledOff(led_ST1);
		ledON(LED_ONLINE);
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		display_7seg_displayErr();
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL
		status_leds_ledOff(led_ST3);

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
	status_leds_ledOff(led_ST3);
	#endif // DEBUG_NONETWORK
}