/*
 * scheduler.h
 *
 *  Created on: Nov 28, 2022
 *      Author: Nguyen Duy Ha
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include "uart.h"
//#include "llist.h"
#include "stdio.h"
#include <stdint.h>

typedef struct{
	void(*pTask)(uint32_t);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
	uint32_t led_index;
} sTasks;

extern uint32_t tick;

#define SCH_MAX_TASKS	40

void SCH_Init(void);

uint32_t SCH_Add_Task(void (*pFunction)(uint32_t), uint32_t, uint32_t, uint32_t);

uint8_t SCH_Delete_Task(uint32_t);

void SCH_Update(void);

void SCH_Dispatch_Tasks(void);

#endif /* INC_SCHEDULER_H_ */

