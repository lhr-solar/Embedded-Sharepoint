

# File freertos.c



[**FileList**](files.md) **>** [**common**](dir_bdd9a5d540de89e9fe90efdfc6973a4f.md) **>** [**Src**](dir_a68c9adcdd2c4e614c81801efca3e79f.md) **>** [**freertos.c**](freertos_8c.md)

[Go to the source code of this file](freertos_8c_source.md)



* `#include "FreeRTOS.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  StackType\_t | [**xIdleStack**](#variable-xidlestack)  <br> |
|  StaticTask\_t | [**xIdleTaskTCBBuffer**](#variable-xidletasktcbbuffer)  <br> |
|  StackType\_t | [**xTimerStack**](#variable-xtimerstack)  <br> |
|  StaticTask\_t | [**xTimerTaskTCBBuffer**](#variable-xtimertasktcbbuffer)  <br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**vApplicationGetIdleTaskMemory**](#function-vapplicationgetidletaskmemory) (StaticTask\_t \*\* ppxIdleTaskTCBBuffer, StackType\_t \*\* ppxIdleTaskStackBuffer, uint32\_t \* pulIdleTaskStackSize) <br>_Create memory for Idle task._  |
|  void | [**vApplicationGetTimerTaskMemory**](#function-vapplicationgettimertaskmemory) (StaticTask\_t \*\* ppxTimerTaskTCBBuffer, StackType\_t \*\* ppxTimerTaskStackBuffer, uint32\_t \* pulTimerTaskStackSize) <br>_Create memory for Timer task._  |




























## Public Static Attributes Documentation




### variable xIdleStack 

```C++
StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
```




<hr>



### variable xIdleTaskTCBBuffer 

```C++
StaticTask_t xIdleTaskTCBBuffer;
```




<hr>



### variable xTimerStack 

```C++
StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];
```




<hr>



### variable xTimerTaskTCBBuffer 

```C++
StaticTask_t xTimerTaskTCBBuffer;
```




<hr>
## Public Functions Documentation




### function vApplicationGetIdleTaskMemory 

_Create memory for Idle task._ 
```C++
void vApplicationGetIdleTaskMemory (
    StaticTask_t ** ppxIdleTaskTCBBuffer,
    StackType_t ** ppxIdleTaskStackBuffer,
    uint32_t * pulIdleTaskStackSize
) 
```





**Parameters:**


* `ppxIdleTaskTCBBuffer` 
* `ppxIdleTaskStackBuffer` 
* `pulIdleTaskStackSize` 




        

<hr>



### function vApplicationGetTimerTaskMemory 

_Create memory for Timer task._ 
```C++
void vApplicationGetTimerTaskMemory (
    StaticTask_t ** ppxTimerTaskTCBBuffer,
    StackType_t ** ppxTimerTaskStackBuffer,
    uint32_t * pulTimerTaskStackSize
) 
```





**Parameters:**


* `ppxTimerTaskTCBBuffer` 
* `ppxTimerTaskStackBuffer` 
* `pulTimerTaskStackSize` 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `common/Src/freertos.c`

