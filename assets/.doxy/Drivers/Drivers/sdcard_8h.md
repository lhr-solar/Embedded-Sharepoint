

# File sdcard.h



[**FileList**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**sdcard.h**](sdcard_8h.md)

[Go to the source code of this file](sdcard_8h_source.md)



* `#include "stm32xx_hal.h"`
* `#include <stdint.h>`
* `#include <stddef.h>`
* `#include "FreeRTOS.h"`
* `#include "semphr.h"`
* `#include "queue.h"`
* `#include "queue_ex.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**sd\_handle\_t**](structsd__handle__t.md) <br>_Main Handle Structure for SD Card._  |
| struct | [**sd\_job\_t**](structsd__job__t.md) <br>_Job structure passed through the RTOS Queue._  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**sd\_job\_type\_t**](#enum-sd_job_type_t)  <br>_Enum for Worker Task Job Types._  |
| enum  | [**sd\_status\_t**](#enum-sd_status_t)  <br>_Custom error status enum._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**sd\_status\_t**](sdcard_8h.md#enum-sd_status_t) | [**SD\_Init**](#function-sd_init) ([**sd\_handle\_t**](structsd__handle__t.md) \* sd, TickType\_t timeout) <br>_Low-level SD initialization to ready the card for SPI communication._  |
|  [**sd\_status\_t**](sdcard_8h.md#enum-sd_status_t) | [**SD\_ReadSingleBlock**](#function-sd_readsingleblock) ([**sd\_handle\_t**](structsd__handle__t.md) \* sd, uint32\_t blockNum, uint8\_t \* buffer, TickType\_t timeout) <br>_Reads a single 512-byte block from the SD card._  |
|  [**sd\_status\_t**](sdcard_8h.md#enum-sd_status_t) | [**SD\_WriteSingleBlock**](#function-sd_writesingleblock) ([**sd\_handle\_t**](structsd__handle__t.md) \* sd, uint32\_t blockNum, const uint8\_t \* buffer, TickType\_t timeout) <br>_Writes a single 512-byte block to the SD card._  |
|  [**sd\_status\_t**](sdcard_8h.md#enum-sd_status_t) | [**USER\_SD\_Card\_Init**](#function-user_sd_card_init) ([**sd\_handle\_t**](structsd__handle__t.md) \* sd, UBaseType\_t priority) <br>_Initializes RTOS resources and FatFs structures for the SD driver._  |
|  void | [**USER\_SD\_Card\_Worker\_Task**](#function-user_sd_card_worker_task) (void \* params) <br>_RTOS task that processes file system jobs from the queue._  |
|  [**sd\_status\_t**](sdcard_8h.md#enum-sd_status_t) | [**USER\_SD\_Card\_Write\_Async**](#function-user_sd_card_write_async) ([**sd\_handle\_t**](structsd__handle__t.md) \* sd, const char \* filename, const char \* data, TickType\_t delay\_ticks) <br>_Non-blocking write that sends a request to the background worker._  |
|  void | [**sdcard\_SPI\_TxRxCpltCallback**](#function-sdcard_spi_txrxcpltcallback) (SPI\_HandleTypeDef \* hspi, BaseType\_t \* xHigherPriorityTaskWoken) <br>_Driver-specific callback triggered by the SPI ISR._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**DATA\_TOKEN\_CMD18**](sdcard_8h.md#define-data_token_cmd18)  `0xFE`<br> |
| define  | [**DATA\_TOKEN\_CMD25**](sdcard_8h.md#define-data_token_cmd25)  `0xFC`<br> |
| define  | [**SD\_ACMD41**](sdcard_8h.md#define-sd_acmd41)  `41`<br> |
| define  | [**SD\_BLOCK\_SIZE**](sdcard_8h.md#define-sd_block_size)  `512`<br> |
| define  | [**SD\_BUSY\_WAIT\_RETRIES**](sdcard_8h.md#define-sd_busy_wait_retries)  `500`<br> |
| define  | [**SD\_BYTE\_MASK**](sdcard_8h.md#define-sd_byte_mask)  `0xFF`<br> |
| define  | [**SD\_BYTE\_SHIFT\_16**](sdcard_8h.md#define-sd_byte_shift_16)  `16`<br> |
| define  | [**SD\_BYTE\_SHIFT\_24**](sdcard_8h.md#define-sd_byte_shift_24)  `24`<br> |
| define  | [**SD\_BYTE\_SHIFT\_8**](sdcard_8h.md#define-sd_byte_shift_8)  `8`<br> |
| define  | [**SD\_CMD0**](sdcard_8h.md#define-sd_cmd0)  `0`<br> |
| define  | [**SD\_CMD12**](sdcard_8h.md#define-sd_cmd12)  `12`<br> |
| define  | [**SD\_CMD17**](sdcard_8h.md#define-sd_cmd17)  `17`<br> |
| define  | [**SD\_CMD18**](sdcard_8h.md#define-sd_cmd18)  `18`<br> |
| define  | [**SD\_CMD24**](sdcard_8h.md#define-sd_cmd24)  `24`<br> |
| define  | [**SD\_CMD25**](sdcard_8h.md#define-sd_cmd25)  `25`<br> |
| define  | [**SD\_CMD55**](sdcard_8h.md#define-sd_cmd55)  `55`<br> |
| define  | [**SD\_CMD58**](sdcard_8h.md#define-sd_cmd58)  `58`<br> |
| define  | [**SD\_CMD8**](sdcard_8h.md#define-sd_cmd8)  `8`<br> |
| define  | [**SD\_CMD\_BASE**](sdcard_8h.md#define-sd_cmd_base)  `0x40`<br> |
| define  | [**SD\_CMD\_INDEX\_MASK**](sdcard_8h.md#define-sd_cmd_index_mask)  `0x3F`<br> |
| define  | [**SD\_CMD\_PACKET\_SIZE**](sdcard_8h.md#define-sd_cmd_packet_size)  `6`<br> |
| define  | [**SD\_DATA\_BUFFER\_LEN**](sdcard_8h.md#define-sd_data_buffer_len)  `64`<br> |
| define  | [**SD\_DEFAULT\_SECTOR\_COUNT**](sdcard_8h.md#define-sd_default_sector_count)  `3934208`<br> |
| define  | [**SD\_DEFAULT\_TIMEOUT\_MS**](sdcard_8h.md#define-sd_default_timeout_ms)  `1000`<br> |
| define  | [**SD\_DUMMY\_BYTE**](sdcard_8h.md#define-sd_dummy_byte)  `0xFF`<br> |
| define  | [**SD\_DUMMY\_CLOCKS\_COUNT**](sdcard_8h.md#define-sd_dummy_clocks_count)  `10`<br> |
| define  | [**SD\_MAX\_FILENAME\_LEN**](sdcard_8h.md#define-sd_max_filename_len)  `13`<br> |
| define  | [**SD\_QUEUE\_LENGTH**](sdcard_8h.md#define-sd_queue_length)  `5`<br> |
| define  | [**SD\_R1\_ERROR\_CODE**](sdcard_8h.md#define-sd_r1_error_code)  `0xFF`<br> |
| define  | [**SD\_R1\_IDLE\_MASK**](sdcard_8h.md#define-sd_r1_idle_mask)  `0x80`<br> |
| define  | [**SD\_R1\_POLL\_RETRIES**](sdcard_8h.md#define-sd_r1_poll_retries)  `8`<br> |
| define  | [**SD\_SMALL\_TRANSFER\_THRESHOLD**](sdcard_8h.md#define-sd_small_transfer_threshold)  `16`<br> |
| define  | [**SD\_TIMEOUT\_MS**](sdcard_8h.md#define-sd_timeout_ms)  `1000`<br> |

## Public Types Documentation




### enum sd\_job\_type\_t 

_Enum for Worker Task Job Types._ 
```C++
enum sd_job_type_t {
    SD_JOB_MOUNT,
    SD_JOB_WRITE_ASYNC,
    SD_JOB_FORMAT
};
```




<hr>



### enum sd\_status\_t 

_Custom error status enum._ 
```C++
enum sd_status_t {
    SD_OK = 0x00,
    SD_ERROR = 0x01,
    SD_TIMEOUT = 0x02,
    SD_ERR_SPI = 0x03,
    SD_ERR_INIT_CARD = 0x04,
    SD_ERR_VOLTAGE = 0x05,
    SD_ERR_READ = 0x06,
    SD_ERR_WRITE = 0x07,
    SD_ERR_MUTEX = 0x08,
    SD_ERR_QUEUE_FULL = 0x09,
    SD_ERR_LOCK_TIMEOUT = 0x0A
};
```




<hr>
## Public Functions Documentation




### function SD\_Init 

_Low-level SD initialization to ready the card for SPI communication._ 
```C++
sd_status_t SD_Init (
    sd_handle_t * sd,
    TickType_t timeout
) 
```





**Parameters:**


* `sd` Pointer to the SD handle. 
* `timeout` Max time to wait for initialization. 



**Returns:**

Returns SD\_OK on success. 





        

<hr>



### function SD\_ReadSingleBlock 

_Reads a single 512-byte block from the SD card._ 
```C++
sd_status_t SD_ReadSingleBlock (
    sd_handle_t * sd,
    uint32_t blockNum,
    uint8_t * buffer,
    TickType_t timeout
) 
```





**Parameters:**


* `sd` Pointer to the SD handle. 
* `blockNum` Sector address to read. 
* `buffer` Pointer to the 512-byte data buffer. 
* `timeout` Max time to wait for the block. 



**Returns:**

Returns SD\_OK on success. 





        

<hr>



### function SD\_WriteSingleBlock 

_Writes a single 512-byte block to the SD card._ 
```C++
sd_status_t SD_WriteSingleBlock (
    sd_handle_t * sd,
    uint32_t blockNum,
    const uint8_t * buffer,
    TickType_t timeout
) 
```





**Parameters:**


* `sd` Pointer to the SD handle. 
* `blockNum` Sector address to write. 
* `buffer` Pointer to the 512-byte data buffer. 
* `timeout` Max time to wait for the write to finish. 



**Returns:**

Returns SD\_OK on success. 





        

<hr>



### function USER\_SD\_Card\_Init 

_Initializes RTOS resources and FatFs structures for the SD driver._ 
```C++
sd_status_t USER_SD_Card_Init (
    sd_handle_t * sd,
    UBaseType_t priority
) 
```





**Parameters:**


* `sd` Pointer to the SD handle. 
* `priority` User choice of priority level 



**Returns:**

Returns SD\_OK on success. 





        

<hr>



### function USER\_SD\_Card\_Worker\_Task 

_RTOS task that processes file system jobs from the queue._ 
```C++
void USER_SD_Card_Worker_Task (
    void * params
) 
```





**Parameters:**


* `params` Pointer to the [**sd\_handle\_t**](structsd__handle__t.md) structure. 




        

<hr>



### function USER\_SD\_Card\_Write\_Async 

_Non-blocking write that sends a request to the background worker._ 
```C++
sd_status_t USER_SD_Card_Write_Async (
    sd_handle_t * sd,
    const char * filename,
    const char * data,
    TickType_t delay_ticks
) 
```





**Parameters:**


* `sd` Pointer to the SD handle. 
* `filename` Name of the file to write to. 
* `data` String data to append. 
* `delay_ticks` Time to wait for queue space if full. 



**Returns:**

Returns SD\_OK if the job was queued. 





        

<hr>



### function sdcard\_SPI\_TxRxCpltCallback 

_Driver-specific callback triggered by the SPI ISR._ 
```C++
void sdcard_SPI_TxRxCpltCallback (
    SPI_HandleTypeDef * hspi,
    BaseType_t * xHigherPriorityTaskWoken
) 
```





**Parameters:**


* `hspi` Pointer to the HAL SPI handle that triggered the interrupt. 
* `xHigherPriorityTaskWoken` Pointer to a flag indicating if a context switch is required. 




        

<hr>
## Macro Definition Documentation





### define DATA\_TOKEN\_CMD18 

```C++
#define DATA_TOKEN_CMD18 `0xFE`
```




<hr>



### define DATA\_TOKEN\_CMD25 

```C++
#define DATA_TOKEN_CMD25 `0xFC`
```




<hr>



### define SD\_ACMD41 

```C++
#define SD_ACMD41 `41`
```




<hr>



### define SD\_BLOCK\_SIZE 

```C++
#define SD_BLOCK_SIZE `512`
```




<hr>



### define SD\_BUSY\_WAIT\_RETRIES 

```C++
#define SD_BUSY_WAIT_RETRIES `500`
```




<hr>



### define SD\_BYTE\_MASK 

```C++
#define SD_BYTE_MASK `0xFF`
```




<hr>



### define SD\_BYTE\_SHIFT\_16 

```C++
#define SD_BYTE_SHIFT_16 `16`
```




<hr>



### define SD\_BYTE\_SHIFT\_24 

```C++
#define SD_BYTE_SHIFT_24 `24`
```




<hr>



### define SD\_BYTE\_SHIFT\_8 

```C++
#define SD_BYTE_SHIFT_8 `8`
```




<hr>



### define SD\_CMD0 

```C++
#define SD_CMD0 `0`
```




<hr>



### define SD\_CMD12 

```C++
#define SD_CMD12 `12`
```




<hr>



### define SD\_CMD17 

```C++
#define SD_CMD17 `17`
```




<hr>



### define SD\_CMD18 

```C++
#define SD_CMD18 `18`
```




<hr>



### define SD\_CMD24 

```C++
#define SD_CMD24 `24`
```




<hr>



### define SD\_CMD25 

```C++
#define SD_CMD25 `25`
```




<hr>



### define SD\_CMD55 

```C++
#define SD_CMD55 `55`
```




<hr>



### define SD\_CMD58 

```C++
#define SD_CMD58 `58`
```




<hr>



### define SD\_CMD8 

```C++
#define SD_CMD8 `8`
```




<hr>



### define SD\_CMD\_BASE 

```C++
#define SD_CMD_BASE `0x40`
```




<hr>



### define SD\_CMD\_INDEX\_MASK 

```C++
#define SD_CMD_INDEX_MASK `0x3F`
```




<hr>



### define SD\_CMD\_PACKET\_SIZE 

```C++
#define SD_CMD_PACKET_SIZE `6`
```




<hr>



### define SD\_DATA\_BUFFER\_LEN 

```C++
#define SD_DATA_BUFFER_LEN `64`
```




<hr>



### define SD\_DEFAULT\_SECTOR\_COUNT 

```C++
#define SD_DEFAULT_SECTOR_COUNT `3934208`
```




<hr>



### define SD\_DEFAULT\_TIMEOUT\_MS 

```C++
#define SD_DEFAULT_TIMEOUT_MS `1000`
```




<hr>



### define SD\_DUMMY\_BYTE 

```C++
#define SD_DUMMY_BYTE `0xFF`
```




<hr>



### define SD\_DUMMY\_CLOCKS\_COUNT 

```C++
#define SD_DUMMY_CLOCKS_COUNT `10`
```




<hr>



### define SD\_MAX\_FILENAME\_LEN 

```C++
#define SD_MAX_FILENAME_LEN `13`
```




<hr>



### define SD\_QUEUE\_LENGTH 

```C++
#define SD_QUEUE_LENGTH `5`
```




<hr>



### define SD\_R1\_ERROR\_CODE 

```C++
#define SD_R1_ERROR_CODE `0xFF`
```




<hr>



### define SD\_R1\_IDLE\_MASK 

```C++
#define SD_R1_IDLE_MASK `0x80`
```




<hr>



### define SD\_R1\_POLL\_RETRIES 

```C++
#define SD_R1_POLL_RETRIES `8`
```




<hr>



### define SD\_SMALL\_TRANSFER\_THRESHOLD 

```C++
#define SD_SMALL_TRANSFER_THRESHOLD `16`
```




<hr>



### define SD\_TIMEOUT\_MS 

```C++
#define SD_TIMEOUT_MS `1000`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/sdcard.h`

