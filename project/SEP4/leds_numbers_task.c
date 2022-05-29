/*
 * leds_numbers_task.c
 *
 * Created: 5/27/2022 5:46:14 PM
 *  Author: nordesk
 */ 

#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>
#include <xevent_groups.h>

#include <program_config.h>


#define LED_POWER LED_501
#define LED_APP_TASK LED_502
#define LED_CO2_TASK LED_504
#define LED_THF_TASK LED_505
#define LED_LUX_TASK LED_506
#define LED_ERROR LED_508

#define LED_501 PA0
#define LED_502 PA1
#define LED_503 PA2
#define LED_504 PA3
#define LED_505 PA4
#define LED_506 PA5
#define LED_507 PA6
#define LED_508 PA7

#define BIT_LED(x) (1 << x)

#define LED_STATUS_OFF 0
#define LED_STATUS_ON 1
#define LED_STATUS_BLINKS 2
#define LED_STATUS_BLINKF 3

short _LED_STATUS[8];
short _LED_BLINK_STATUS[8];

void initLEDs()
{
		int j;
		for(j = 1; j <= 7; j++){
			_LED_STATUS[j]=0;
		}
		_LED_STATUS[0] = 1;
		xEventGroupSetBits(getNumbersEventGroup(), BIT_LED(1) | BIT_LED(2) | BIT_LED(3) | BIT_LED(4) | BIT_LED(5) | BIT_LED(6) | BIT_LED(7) );
		printf("LED OK\n");
}

void triggerEvent(short bit_led){
	xEventGroupSetBits(getNumbersEventGroup(), BIT_LED(bit_led));
}

void ledOFF(short _led){
	_LED_STATUS[_led] = LED_STATUS_OFF;
	triggerEvent(_led);
}

void ledON(short _led){
	_LED_STATUS[_led] = LED_STATUS_ON;
	triggerEvent(_led);
}

void ledToggle(short _led){
	if(_LED_STATUS[_led] > 0){
		_LED_STATUS[_led] = 0;
	} else {
		_LED_STATUS[_led] = 1;	
	}
	triggerEvent(_led);
}

void ledBlinkShort(short _led){
	_LED_STATUS[_led] = LED_STATUS_BLINKS;
	triggerEvent(_led);
}

void ledBlinkFast(short _led){
	_LED_STATUS[_led] = LED_STATUS_BLINKF;
	triggerEvent(_led);
}

void _check(int i)
{
	switch(_LED_STATUS[i]){
		case LED_STATUS_BLINKS: {
			if(_LED_BLINK_STATUS[i] > 1) {
				PORTA ^= (1<<i);
				_LED_BLINK_STATUS[i] = 0;
				} else {
				_LED_BLINK_STATUS[i]++;
			}
			break;
			
		}
		case LED_STATUS_BLINKF: {
			if(_LED_BLINK_STATUS[i] > 0) {
				PORTA ^= (1<<i);
				_LED_BLINK_STATUS[i] = 0;
				} else {
				_LED_BLINK_STATUS[i]++;
			}
			break;
		}
		case LED_STATUS_ON: {
			PORTA &= ~(1<<i);
			_LED_STATUS[i] = -1;
			break;
		}
		case LED_STATUS_OFF: {
			PORTA |= (1<<i);
			_LED_STATUS[i] = -1;
			break;
		}
	}
}

void ledsAndNumbersTask(void *pvParameters){
	initLEDs();
	int i;
	EventBits_t eventBits;
	for(;;){
		eventBits = xEventGroupWaitBits(getNumbersEventGroup(),
		(1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7),
		pdTRUE,
		pdFALSE,
		SETTING_TIMEOUT_WAIT_LED_BITS/portTICK_PERIOD_MS);
		if(eventBits & (1 << 1)){
			i = 1;
			_check(i);
		} if(eventBits & (1 << 2)){
			i = 2;
			_check(i);
		} if(eventBits & (1 << 3)){
			i = 3;
			_check(i);
		} if(eventBits & (1 << 4)){
			i = 4;
			_check(i);
		} if(eventBits & (1 << 5)){
			i = 5;
			_check(i);
		} if(eventBits & (1 << 6)){
			i = 6;
			_check(i);
		} if(eventBits & (1 << 7)){
			i = 7;
			_check(i);
		}
	}
}