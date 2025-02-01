#ifndef PWM_TASK_H_
#define PWM_TASK_H_

#include "FreeRTOS.h" /* Must come first. */
#include "task.h" /* RTOS task related API prototypes. */
#include "queue.h" /* RTOS queue related API prototypes. */
#include "timers.h" /* Software timer related API prototypes. */
#include "semphr.h"

void Task_Init();
void Task_Toggle();
void Task_TIM1();
void Task_TIM2();

#endif