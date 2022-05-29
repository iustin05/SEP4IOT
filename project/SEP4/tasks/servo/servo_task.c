/*
 * servo_task.c
 *
 * Created: 5/26/2022 1:54:54 AM
 *  Author: nordesk
 */
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>

#include <xevent_groups.h>

#include <configuration.h>

#include <program_config.h>

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
		xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_SERVO,
		pdTRUE,
		pdTRUE,
		20000/portTICK_PERIOD_MS);
		// xTaskDelayUntil( &xLastWakeTime, xFrequency );
		// vTaskDelay(SETTING_CYCLE_SERVO/portTICK_PERIOD_MS);
		ledON(LED_SERVO);
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
				vTaskDelay(10);
				if(current_angle == last_angle){
					
				} else {
					printf("Servo changed; correcting angle\n");
					rc_servo_setPosition(SERVO_J13, current_angle);
					last_angle = current_angle;
					char str[3];
					sprintf(str, "%d", current_angle);
					display_7seg_displayHex(str);
				}
					//printf("Servo changed; correcting angle\n");
					//rc_servo_setPosition(SERVO_J13, current_angle);
					//last_angle = current_angle;
					//char str[4];
					//sprintf(str, "%d\n", current_angle);
					//display_7seg_displayHex(str);
				
			}
			else
			{
				printf("Error receiving servo angle.\n");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
		vTaskDelay(100/portTICK_PERIOD_MS);
		ledOFF(LED_SERVO);
		}
}