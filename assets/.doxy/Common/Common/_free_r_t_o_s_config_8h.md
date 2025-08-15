

# File FreeRTOSConfig.h



[**FileList**](files.md) **>** [**common**](dir_bdd9a5d540de89e9fe90efdfc6973a4f.md) **>** [**Inc**](dir_4152f073bf32bf79124b676c9fe2adc1.md) **>** [**FreeRTOSConfig.h**](_free_r_t_o_s_config_8h.md)

[Go to the source code of this file](_free_r_t_o_s_config_8h_source.md)



































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**INCLUDE\_uxTaskPriorityGet**](_free_r_t_o_s_config_8h.md#define-include_uxtaskpriorityget)  `1`<br> |
| define  | [**INCLUDE\_vTaskCleanUpResources**](_free_r_t_o_s_config_8h.md#define-include_vtaskcleanupresources)  `0`<br> |
| define  | [**INCLUDE\_vTaskDelay**](_free_r_t_o_s_config_8h.md#define-include_vtaskdelay)  `1`<br> |
| define  | [**INCLUDE\_vTaskDelayUntil**](_free_r_t_o_s_config_8h.md#define-include_vtaskdelayuntil)  `1`<br> |
| define  | [**INCLUDE\_vTaskDelete**](_free_r_t_o_s_config_8h.md#define-include_vtaskdelete)  `1`<br> |
| define  | [**INCLUDE\_vTaskPrioritySet**](_free_r_t_o_s_config_8h.md#define-include_vtaskpriorityset)  `1`<br> |
| define  | [**INCLUDE\_vTaskSuspend**](_free_r_t_o_s_config_8h.md#define-include_vtasksuspend)  `1`<br> |
| define  | [**INCLUDE\_xTaskGetSchedulerState**](_free_r_t_o_s_config_8h.md#define-include_xtaskgetschedulerstate)  `1`<br> |
| define  | [**configASSERT**](_free_r_t_o_s_config_8h.md#define-configassert) (x) `if ((x) == 0) {taskDISABLE\_INTERRUPTS(); for( ;; );}`<br> |
| define  | [**configCPU\_CLOCK\_HZ**](_free_r_t_o_s_config_8h.md#define-configcpu_clock_hz)  `( SystemCoreClock )`<br> |
| define  | [**configENABLE\_FPU**](_free_r_t_o_s_config_8h.md#define-configenable_fpu)  `1`<br> |
| define  | [**configENABLE\_MPU**](_free_r_t_o_s_config_8h.md#define-configenable_mpu)  `0`<br> |
| define  | [**configKERNEL\_INTERRUPT\_PRIORITY**](_free_r_t_o_s_config_8h.md#define-configkernel_interrupt_priority)  `( configLIBRARY\_LOWEST\_INTERRUPT\_PRIORITY &lt;&lt; (8 - configPRIO\_BITS) )`<br> |
| define  | [**configLIBRARY\_LOWEST\_INTERRUPT\_PRIORITY**](_free_r_t_o_s_config_8h.md#define-configlibrary_lowest_interrupt_priority)  `15`<br> |
| define  | [**configLIBRARY\_MAX\_SYSCALL\_INTERRUPT\_PRIORITY**](_free_r_t_o_s_config_8h.md#define-configlibrary_max_syscall_interrupt_priority)  `5`<br> |
| define  | [**configMAX\_CO\_ROUTINE\_PRIORITIES**](_free_r_t_o_s_config_8h.md#define-configmax_co_routine_priorities)  `( 2 )`<br> |
| define  | [**configMAX\_PRIORITIES**](_free_r_t_o_s_config_8h.md#define-configmax_priorities)  `( 7 )`<br> |
| define  | [**configMAX\_SYSCALL\_INTERRUPT\_PRIORITY**](_free_r_t_o_s_config_8h.md#define-configmax_syscall_interrupt_priority)  `( configLIBRARY\_MAX\_SYSCALL\_INTERRUPT\_PRIORITY &lt;&lt; (8 - configPRIO\_BITS) )`<br> |
| define  | [**configMAX\_TASK\_NAME\_LEN**](_free_r_t_o_s_config_8h.md#define-configmax_task_name_len)  `( 16 )`<br> |
| define  | [**configMESSAGE\_BUFFER\_LENGTH\_TYPE**](_free_r_t_o_s_config_8h.md#define-configmessage_buffer_length_type)  `size\_t`<br> |
| define  | [**configMINIMAL\_STACK\_SIZE**](_free_r_t_o_s_config_8h.md#define-configminimal_stack_size)  `((uint16\_t)128)`<br> |
| define  | [**configPRIO\_BITS**](_free_r_t_o_s_config_8h.md#define-configprio_bits)  `4`<br> |
| define  | [**configQUEUE\_REGISTRY\_SIZE**](_free_r_t_o_s_config_8h.md#define-configqueue_registry_size)  `8`<br> |
| define  | [**configSUPPORT\_DYNAMIC\_ALLOCATION**](_free_r_t_o_s_config_8h.md#define-configsupport_dynamic_allocation)  `0`<br> |
| define  | [**configSUPPORT\_STATIC\_ALLOCATION**](_free_r_t_o_s_config_8h.md#define-configsupport_static_allocation)  `1`<br> |
| define  | [**configTICK\_RATE\_HZ**](_free_r_t_o_s_config_8h.md#define-configtick_rate_hz)  `((TickType\_t)1000)`<br> |
| define  | [**configTIMER\_QUEUE\_LENGTH**](_free_r_t_o_s_config_8h.md#define-configtimer_queue_length)  `10`<br> |
| define  | [**configTIMER\_TASK\_PRIORITY**](_free_r_t_o_s_config_8h.md#define-configtimer_task_priority)  `( 2 )`<br> |
| define  | [**configTIMER\_TASK\_STACK\_DEPTH**](_free_r_t_o_s_config_8h.md#define-configtimer_task_stack_depth)  `256`<br> |
| define  | [**configUSE\_16\_BIT\_TICKS**](_free_r_t_o_s_config_8h.md#define-configuse_16_bit_ticks)  `0`<br> |
| define  | [**configUSE\_COUNTING\_SEMAPHORES**](_free_r_t_o_s_config_8h.md#define-configuse_counting_semaphores)  `1`<br> |
| define  | [**configUSE\_CO\_ROUTINES**](_free_r_t_o_s_config_8h.md#define-configuse_co_routines)  `0`<br> |
| define  | [**configUSE\_IDLE\_HOOK**](_free_r_t_o_s_config_8h.md#define-configuse_idle_hook)  `0`<br> |
| define  | [**configUSE\_MUTEXES**](_free_r_t_o_s_config_8h.md#define-configuse_mutexes)  `1`<br> |
| define  | [**configUSE\_PORT\_OPTIMISED\_TASK\_SELECTION**](_free_r_t_o_s_config_8h.md#define-configuse_port_optimised_task_selection)  `1`<br> |
| define  | [**configUSE\_PREEMPTION**](_free_r_t_o_s_config_8h.md#define-configuse_preemption)  `1`<br> |
| define  | [**configUSE\_RECURSIVE\_MUTEXES**](_free_r_t_o_s_config_8h.md#define-configuse_recursive_mutexes)  `1`<br> |
| define  | [**configUSE\_TICK\_HOOK**](_free_r_t_o_s_config_8h.md#define-configuse_tick_hook)  `0`<br> |
| define  | [**configUSE\_TIMERS**](_free_r_t_o_s_config_8h.md#define-configuse_timers)  `1`<br> |
| define  | [**vPortSVCHandler**](_free_r_t_o_s_config_8h.md#define-vportsvchandler)  `SVC\_Handler`<br> |
| define  | [**xPortPendSVHandler**](_free_r_t_o_s_config_8h.md#define-xportpendsvhandler)  `PendSV\_Handler`<br> |
| define  | [**xPortSysTickHandler**](_free_r_t_o_s_config_8h.md#define-xportsystickhandler)  `SysTick\_Handler`<br> |

## Macro Definition Documentation





### define INCLUDE\_uxTaskPriorityGet 

```C++
#define INCLUDE_uxTaskPriorityGet `1`
```




<hr>



### define INCLUDE\_vTaskCleanUpResources 

```C++
#define INCLUDE_vTaskCleanUpResources `0`
```




<hr>



### define INCLUDE\_vTaskDelay 

```C++
#define INCLUDE_vTaskDelay `1`
```




<hr>



### define INCLUDE\_vTaskDelayUntil 

```C++
#define INCLUDE_vTaskDelayUntil `1`
```




<hr>



### define INCLUDE\_vTaskDelete 

```C++
#define INCLUDE_vTaskDelete `1`
```




<hr>



### define INCLUDE\_vTaskPrioritySet 

```C++
#define INCLUDE_vTaskPrioritySet `1`
```




<hr>



### define INCLUDE\_vTaskSuspend 

```C++
#define INCLUDE_vTaskSuspend `1`
```




<hr>



### define INCLUDE\_xTaskGetSchedulerState 

```C++
#define INCLUDE_xTaskGetSchedulerState `1`
```




<hr>



### define configASSERT 

```C++
#define configASSERT (
    x
) `if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}`
```




<hr>



### define configCPU\_CLOCK\_HZ 

```C++
#define configCPU_CLOCK_HZ `( SystemCoreClock )`
```




<hr>



### define configENABLE\_FPU 

```C++
#define configENABLE_FPU `1`
```




<hr>



### define configENABLE\_MPU 

```C++
#define configENABLE_MPU `0`
```




<hr>



### define configKERNEL\_INTERRUPT\_PRIORITY 

```C++
#define configKERNEL_INTERRUPT_PRIORITY `( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )`
```




<hr>



### define configLIBRARY\_LOWEST\_INTERRUPT\_PRIORITY 

```C++
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY `15`
```




<hr>



### define configLIBRARY\_MAX\_SYSCALL\_INTERRUPT\_PRIORITY 

```C++
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY `5`
```




<hr>



### define configMAX\_CO\_ROUTINE\_PRIORITIES 

```C++
#define configMAX_CO_ROUTINE_PRIORITIES `( 2 )`
```




<hr>



### define configMAX\_PRIORITIES 

```C++
#define configMAX_PRIORITIES `( 7 )`
```




<hr>



### define configMAX\_SYSCALL\_INTERRUPT\_PRIORITY 

```C++
#define configMAX_SYSCALL_INTERRUPT_PRIORITY `( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )`
```




<hr>



### define configMAX\_TASK\_NAME\_LEN 

```C++
#define configMAX_TASK_NAME_LEN `( 16 )`
```




<hr>



### define configMESSAGE\_BUFFER\_LENGTH\_TYPE 

```C++
#define configMESSAGE_BUFFER_LENGTH_TYPE `size_t`
```




<hr>



### define configMINIMAL\_STACK\_SIZE 

```C++
#define configMINIMAL_STACK_SIZE `((uint16_t)128)`
```




<hr>



### define configPRIO\_BITS 

```C++
#define configPRIO_BITS `4`
```




<hr>



### define configQUEUE\_REGISTRY\_SIZE 

```C++
#define configQUEUE_REGISTRY_SIZE `8`
```




<hr>



### define configSUPPORT\_DYNAMIC\_ALLOCATION 

```C++
#define configSUPPORT_DYNAMIC_ALLOCATION `0`
```




<hr>



### define configSUPPORT\_STATIC\_ALLOCATION 

```C++
#define configSUPPORT_STATIC_ALLOCATION `1`
```




<hr>



### define configTICK\_RATE\_HZ 

```C++
#define configTICK_RATE_HZ `((TickType_t)1000)`
```




<hr>



### define configTIMER\_QUEUE\_LENGTH 

```C++
#define configTIMER_QUEUE_LENGTH `10`
```




<hr>



### define configTIMER\_TASK\_PRIORITY 

```C++
#define configTIMER_TASK_PRIORITY `( 2 )`
```




<hr>



### define configTIMER\_TASK\_STACK\_DEPTH 

```C++
#define configTIMER_TASK_STACK_DEPTH `256`
```




<hr>



### define configUSE\_16\_BIT\_TICKS 

```C++
#define configUSE_16_BIT_TICKS `0`
```




<hr>



### define configUSE\_COUNTING\_SEMAPHORES 

```C++
#define configUSE_COUNTING_SEMAPHORES `1`
```




<hr>



### define configUSE\_CO\_ROUTINES 

```C++
#define configUSE_CO_ROUTINES `0`
```




<hr>



### define configUSE\_IDLE\_HOOK 

```C++
#define configUSE_IDLE_HOOK `0`
```




<hr>



### define configUSE\_MUTEXES 

```C++
#define configUSE_MUTEXES `1`
```




<hr>



### define configUSE\_PORT\_OPTIMISED\_TASK\_SELECTION 

```C++
#define configUSE_PORT_OPTIMISED_TASK_SELECTION `1`
```




<hr>



### define configUSE\_PREEMPTION 

```C++
#define configUSE_PREEMPTION `1`
```




<hr>



### define configUSE\_RECURSIVE\_MUTEXES 

```C++
#define configUSE_RECURSIVE_MUTEXES `1`
```




<hr>



### define configUSE\_TICK\_HOOK 

```C++
#define configUSE_TICK_HOOK `0`
```




<hr>



### define configUSE\_TIMERS 

```C++
#define configUSE_TIMERS `1`
```




<hr>



### define vPortSVCHandler 

```C++
#define vPortSVCHandler `SVC_Handler`
```




<hr>



### define xPortPendSVHandler 

```C++
#define xPortPendSVHandler `PendSV_Handler`
```




<hr>



### define xPortSysTickHandler 

```C++
#define xPortSysTickHandler `SysTick_Handler`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `common/Inc/FreeRTOSConfig.h`

