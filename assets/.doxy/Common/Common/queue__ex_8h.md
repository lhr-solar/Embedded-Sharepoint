

# File queue\_ex.h



[**FileList**](files.md) **>** [**common**](dir_bdd9a5d540de89e9fe90efdfc6973a4f.md) **>** [**Inc**](dir_4152f073bf32bf79124b676c9fe2adc1.md) **>** [**queue\_ex.h**](queue__ex_8h.md)

[Go to the source code of this file](queue__ex_8h_source.md)



































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**xQueueSendCircularBuffer**](queue__ex_8h.md#define-xqueuesendcircularbuffer) (xQueue, pvItemToQueue, xTicksToWait, QUEUE\_ITEM\_SIZE) `/* multi line expression */`<br> |
| define  | [**xQueueSendCircularBufferFromISR**](queue__ex_8h.md#define-xqueuesendcircularbufferfromisr) (xQueue, pvItemToQueue, pxHigherPriorityTaskWoken, QUEUE\_ITEM\_SIZE) `/* multi line expression */`<br> |

## Macro Definition Documentation





### define xQueueSendCircularBuffer 

```C++
#define xQueueSendCircularBuffer (
    xQueue,
    pvItemToQueue,
    xTicksToWait,
    QUEUE_ITEM_SIZE
) `/* multi line expression */`
```




<hr>



### define xQueueSendCircularBufferFromISR 

```C++
#define xQueueSendCircularBufferFromISR (
    xQueue,
    pvItemToQueue,
    pxHigherPriorityTaskWoken,
    QUEUE_ITEM_SIZE
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `common/Inc/queue_ex.h`

