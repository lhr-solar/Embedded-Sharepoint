#ifndef PWM_TASK_H_
#define PWM_TASK_H_

#include "FreeRTOS.h" /* Must come first. */
#include "task.h" /* RTOS task related API prototypes. */
#include "queue.h" /* RTOS queue related API prototypes. */
#include "timers.h" /* Software timer related API prototypes. */
#include "semphr.h"


#define TASK_INIT_PRIORITY      tskIDLE_PRIORITY + 2
#define TASK_TOGGLE_PRIORITY    tskIDLE_PRIORITY + 2
#define TASK_TIM1_PRIORITY      tskIDLE_PRIORITY + 2
#define TASK_TIM2_PRIORITY      tskIDLE_PRIORITY + 2

void Task_Init();
void Task_Toggle();
void Task_TIM1();
void Task_TIM2();

#endif