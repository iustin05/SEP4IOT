/*
 * app_tasks.h
 *
 * Created: 5/26/2022 1:35:44 AM
 *  Author: nordesk
 */ 


#ifndef APP_TASKS_H_
#define APP_TASKS_H_

// Tasks

extern void appTask(void *pvParameters);

extern void CO2Task(void *pvParameters);

extern void tempHumTask(void *pvParameters);

extern void luxTask(void *pvParameters);

extern void servoTask(void *pvParameters);

extern void upLinkTask(void *pvParameters);

extern void downLinkTask(void *pvParameters);

#endif /* APP_TASKS_H_ */