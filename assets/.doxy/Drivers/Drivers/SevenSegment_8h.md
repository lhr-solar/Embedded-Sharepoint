

# File SevenSegment.h



[**FileList**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**SevenSegment.h**](SevenSegment_8h.md)

[Go to the source code of this file](SevenSegment_8h_source.md)



* `#include "stm32xx_hal.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**led\_pins**](structled__pins.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**display**](#enum-display)  <br> |
| typedef enum [**display**](SevenSegment_8h.md#enum-display) | [**display\_t**](#typedef-display_t)  <br> |
| typedef struct [**led\_pins**](structled__pins.md) | [**led\_t**](#typedef-led_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**SevenSegment\_Init**](#function-sevensegment_init) () <br> |
|  uint8\_t | [**displayNum\_SevenSegment**](#function-displaynum_sevensegment) ([**display\_t**](SevenSegment_8h.md#typedef-display_t) display, uint8\_t number) <br> |
|  uint8\_t | [**displayTwoNum\_SevenSegment**](#function-displaytwonum_sevensegment) (uint8\_t number) <br> |
|  void | [**off\_SevenSegment**](#function-off_sevensegment) (uint8\_t display) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**MAX\_NUM**](SevenSegment_8h.md#define-max_num)  `99`<br> |
| define  | [**NUMBER\_SEGMENTS**](SevenSegment_8h.md#define-number_segments)  `7`<br> |
| define  | [**NUMBER\_SEVEN\_SEGMENTS**](SevenSegment_8h.md#define-number_seven_segments)  `2`<br> |

## Public Types Documentation




### enum display 

```C++
enum display {
    display_0 = 0,
    display_1 = 1,
    display_off = 2
};
```




<hr>



### typedef display\_t 

```C++
typedef enum display display_t;
```




<hr>



### typedef led\_t 

```C++
typedef struct led_pins led_t;
```




<hr>
## Public Functions Documentation




### function SevenSegment\_Init 

```C++
void SevenSegment_Init () 
```




<hr>



### function displayNum\_SevenSegment 

```C++
uint8_t displayNum_SevenSegment (
    display_t display,
    uint8_t number
) 
```




<hr>



### function displayTwoNum\_SevenSegment 

```C++
uint8_t displayTwoNum_SevenSegment (
    uint8_t number
) 
```




<hr>



### function off\_SevenSegment 

```C++
void off_SevenSegment (
    uint8_t display
) 
```




<hr>
## Macro Definition Documentation





### define MAX\_NUM 

```C++
#define MAX_NUM `99`
```




<hr>



### define NUMBER\_SEGMENTS 

```C++
#define NUMBER_SEGMENTS `7`
```




<hr>



### define NUMBER\_SEVEN\_SEGMENTS 

```C++
#define NUMBER_SEVEN_SEGMENTS `2`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/SevenSegment.h`

