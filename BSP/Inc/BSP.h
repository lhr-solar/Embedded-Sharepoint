#ifndef BSP_CONF_H
#define BSP_CONF_H











// BSP-level error information and handling
typedef enum {
    BSP_OK = 0x0,
    BSP_ERROR = 0x1,
    BSP_BUSY = 0x2,
    BSP_TIMEOUT = 0x4
} BSP_Status;

// Macro for turning HAL_StatusTypeDef to our BSP error
#define CONVERT_RETURN(x) (((x) == 0x3U) ? (BSP_TIMEOUT) : (x))
// Macros for interpreting returns
#define HAS_TIMEOUT(x) ((x) & 0x4)
#define HAS_BUSY(x) ((x) & 0x2)
#define HAS_ERROR(x) ((x) & 0x1)
#define IS_OK(x) (!(HAS_TIMEOUT(x) || HAS_BUSY(x) || HAS_ERROR(x)))

#endif