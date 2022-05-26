/*
 * init_sensors.c
 *
 * Created: 5/26/2022 7:46:17 AM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

#include <hih8120.h>
#include <mh_z19.h>
#include <tsl2591.h>
#include <rc_servo.h>

#include <sensors_callback.h>

void initSensorsAndEvents(){
	
}