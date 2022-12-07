/*
 * scheduler.c
 *
 *  Created on: Nov 28, 2022
 *      Author: Nguyen Duy Ha
 */


#include "scheduler.h"

//struct Node TaskArray[SCH_MAX_TASKS + 1];

sTasks SCH_Tasks_G[SCH_MAX_TASKS+1];
uint32_t current_index_task = 0;

char str[50];

void SCH_Init(void){
	current_index_task = 0;
	for (uint32_t i = 0; i <= SCH_MAX_TASKS; i++){
		SCH_Tasks_G[i].pTask = NULL;
		SCH_Tasks_G[i].Delay = -1;
		SCH_Tasks_G[i].Period = -1;
		SCH_Tasks_G[i].RunMe = -1;
		SCH_Tasks_G[i].TaskID = -1;
		SCH_Tasks_G[i].led_index = -1;
	}
}
//
uint32_t SCH_Add_Task (void (*pFunction)(uint32_t), uint32_t led_index, uint32_t DELAY, uint32_t PERIOD){
	if (current_index_task < SCH_MAX_TASKS){
		SCH_Tasks_G[current_index_task].pTask = pFunction;
		SCH_Tasks_G[current_index_task].Delay = DELAY;
		SCH_Tasks_G[current_index_task].Period = PERIOD;
		SCH_Tasks_G[current_index_task].RunMe = 0;
		SCH_Tasks_G[current_index_task].TaskID = current_index_task;
		SCH_Tasks_G[current_index_task].led_index = led_index;
		return current_index_task++;
	}
	return SCH_MAX_TASKS;
}

uint8_t SCH_Delete_Task(uint32_t TASK_ID){
	for (uint32_t i = TASK_ID; i < SCH_MAX_TASKS; i++){
		SCH_Tasks_G[i] = SCH_Tasks_G[i+1];
		current_index_task--;
		return 0;
	}
	return 1;
}
//
void SCH_Update(void){
	for (int i = 0; i < SCH_MAX_TASKS; i++){
		if (SCH_Tasks_G[i].Delay > 0){
			SCH_Tasks_G[i].Delay--;
		}
		else if (SCH_Tasks_G[i].Delay == 0){
			SCH_Tasks_G[i].Delay = SCH_Tasks_G[i].Period;
			SCH_Tasks_G[i].RunMe += 1;
		}
	}
}

void SCH_Dispatch_Tasks(void){
	for (uint32_t i = 0; i < current_index_task; i++){
		if (SCH_Tasks_G[i].RunMe > 0){
			SCH_Tasks_G[i].RunMe--;
			HAL_UART_Transmit(&huart2, (uint8_t*) str, sprintf(str, "Timestamp: %lu\r\n", tick * 10), 1000);
			(*SCH_Tasks_G[i].pTask)(SCH_Tasks_G[i].led_index);
			if (SCH_Tasks_G[i].Period == 0){
				uint8_t del = SCH_Delete_Task(i);
			}
		}
	}
}
