/*
 * servo_task.c
 *
 * Created: 5/26/2022 1:54:54 AM
 *  Author: nordesk
 */
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

#include <configuration.h>

#include <leds_numbers_tasks.h>

#include <display_7seg.h>

#include <rc_servo.h>

#define SERVO_J13 1
#define SERVO_J14 0

int8_t last_angle;

void initServo(){
	rc_servo_initialise();
}
 
void performBasicSetting(){
	for(int i = -100; i<=100 ; i++){
		rc_servo_setPosition(SERVO_J13,i);
		vTaskDelay(1);
	}
	for(int i = -100; i<=100 ; i++){
		rc_servo_setPosition(SERVO_J13,i*(-1));
		vTaskDelay(1);
	}
	rc_servo_setPosition(SERVO_J13,0);
}

void servoTask(void *pvParameters)
{
	//DEBUG performBasicSetting();
	int8_t current_angle;
	for( ;; )
	{
		//xTaskDelayUntil( &xLastWakeTime, xFrequency );
		vTaskDelay(5000/portTICK_PERIOD_MS);
		ledON(LED_SERVO);
		display_7seg_displayHex("5E");
		if( configMutex != NULL )
		{
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 10 ticks to see if it becomes free. */
			if( xSemaphoreTake( configMutex, ( TickType_t ) 10 ) == pdTRUE )
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				
				current_angle = getServoAngle();
				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				xSemaphoreGive( configMutex );
				
				if(current_angle == last_angle){
					
				} else {
					printf("Servo changed; correcting angle\n");
					rc_servo_setPosition(SERVO_J13, current_angle);
					last_angle = current_angle;
					char str[4];
					sprintf(str, "%d", current_angle);
					display_7seg_displayHex(str);
				}
				
			}
			else
			{
				printf("Error receiving servo angle.\n");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
		vTaskDelay(5000/portTICK_PERIOD_MS);
		ledOFF(LED_SERVO);
	}
}