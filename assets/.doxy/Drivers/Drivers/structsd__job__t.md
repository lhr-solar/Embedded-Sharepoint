

# Struct sd\_job\_t



[**ClassList**](annotated.md) **>** [**sd\_job\_t**](structsd__job__t.md)



_Job structure passed through the RTOS Queue._ 

* `#include <sdcard.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  char | [**data**](#variable-data)  <br> |
|  char | [**filename**](#variable-filename)  <br> |
|  uint16\_t | [**len**](#variable-len)  <br> |
|  [**sd\_job\_type\_t**](sdcard_8h.md#enum-sd_job_type_t) | [**type**](#variable-type)  <br> |












































## Public Attributes Documentation




### variable data 

```C++
char sd_job_t::data[SD_DATA_BUFFER_LEN];
```




<hr>



### variable filename 

```C++
char sd_job_t::filename[SD_MAX_FILENAME_LEN];
```




<hr>



### variable len 

```C++
uint16_t sd_job_t::len;
```




<hr>



### variable type 

```C++
sd_job_type_t sd_job_t::type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/sdcard.h`

