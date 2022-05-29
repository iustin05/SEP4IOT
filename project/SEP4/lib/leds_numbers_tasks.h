/*
 * leds_numbers_tasks.h
 *
 * Created: 5/27/2022 5:50:24 PM
 *  Author: nordesk
 */ 


#ifndef LEDS_NUMBERS_TASKS_H_
#define LEDS_NUMBERS_TASKS_H_

#define LED_APP_TASK 1
#define LED_APP_TASK_WORK 2
#define LED_CO2_TASK 3
#define LED_THF_TASK 4
#define LED_LUX_TASK 5
#define LED_SERVO 6
#define LED_ONLINE 7

#define BIT_LED_APP_TASK (1<<1)
#define BIT_LED_APP_TASK_WORK (1<<2)
#define BIT_LED_CO2_TASK (1<<3)
#define BIT_LED_THF_TASK (1<<4)
#define BIT_LED_LUX_TASK (1<<5)
#define BIT_LED_ERROR (1<<7)

extern void initLEDsAndDisplay();

extern void ledOFF(short _led);

extern void ledON(short _led);

extern void ledToggle(short _led);

extern void ledsAndNumbersTask(void *pvParameters);

extern void ledBlinkShort(short _led);

extern void ledBlinkFast(short _led);

#endif /* LEDS_NUMBERS_TASKS_H_ */