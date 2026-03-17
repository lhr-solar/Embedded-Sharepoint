

# Struct sd\_handle\_t



[**ClassList**](annotated.md) **>** [**sd\_handle\_t**](structsd__handle__t.md)



_Main Handle Structure for SD Card._ 

* `#include <sdcard.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint16\_t | [**cs\_pin**](#variable-cs_pin)  <br> |
|  GPIO\_TypeDef \* | [**cs\_port**](#variable-cs_port)  <br> |
|  SPI\_HandleTypeDef \* | [**hspi**](#variable-hspi)  <br> |
|  QueueHandle\_t | [**job\_queue**](#variable-job_queue)  <br> |
|  SemaphoreHandle\_t | [**mutex**](#variable-mutex)  <br> |
|  StaticSemaphore\_t | [**mutexBuffer**](#variable-mutexbuffer)  <br> |
|  StaticQueue\_t | [**queue\_buffer**](#variable-queue_buffer)  <br> |
|  uint8\_t | [**queue\_storage**](#variable-queue_storage)  <br> |
|  StaticSemaphore\_t | [**spi\_comp\_buffer**](#variable-spi_comp_buffer)  <br> |
|  SemaphoreHandle\_t | [**spi\_comp\_sem**](#variable-spi_comp_sem)  <br> |
|  TaskHandle\_t | [**worker\_task\_handle**](#variable-worker_task_handle)  <br> |












































## Public Attributes Documentation




### variable cs\_pin 

```C++
uint16_t sd_handle_t::cs_pin;
```




<hr>



### variable cs\_port 

```C++
GPIO_TypeDef* sd_handle_t::cs_port;
```




<hr>



### variable hspi 

```C++
SPI_HandleTypeDef* sd_handle_t::hspi;
```




<hr>



### variable job\_queue 

```C++
QueueHandle_t sd_handle_t::job_queue;
```




<hr>



### variable mutex 

```C++
SemaphoreHandle_t sd_handle_t::mutex;
```




<hr>



### variable mutexBuffer 

```C++
StaticSemaphore_t sd_handle_t::mutexBuffer;
```




<hr>



### variable queue\_buffer 

```C++
StaticQueue_t sd_handle_t::queue_buffer;
```




<hr>



### variable queue\_storage 

```C++
uint8_t sd_handle_t::queue_storage[SD_QUEUE_LENGTH *sizeof(sd_job_t)];
```




<hr>



### variable spi\_comp\_buffer 

```C++
StaticSemaphore_t sd_handle_t::spi_comp_buffer;
```




<hr>



### variable spi\_comp\_sem 

```C++
SemaphoreHandle_t sd_handle_t::spi_comp_sem;
```




<hr>



### variable worker\_task\_handle 

```C++
TaskHandle_t sd_handle_t::worker_task_handle;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/sdcard.h`

