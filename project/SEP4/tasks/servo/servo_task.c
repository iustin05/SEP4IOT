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

#include <rc_servo.h>

#define SERVO_J13 1
#define SERVO_J14 0

int16_t last_angle = 70;

void initServo(){
	rc_servo_initialise();
}
 
void servoTask(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	int16_t current_angle;
	for( ;; )
	{
		if( configMutex != NULL )
		{
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 10 ticks to see if it becomes free. */
			if( xSemaphoreTake( configMutex, ( TickType_t ) 1000 ) == pdTRUE )
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				
				current_angle = getServoAngle();

				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				xSemaphoreGive( configMutex );
				
				printf("Servo angle received from configuration\n");
				
				if(current_angle == last_angle){
					printf("Servo angle unchanged.\n");
				} else {
					printf("Servo angle changed; correcting angle...");
					rc_servo_setPosition(SERVO_J13, current_angle);
				}
				
			}
			else
			{
				printf("Error receiving servo angle.\n");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}