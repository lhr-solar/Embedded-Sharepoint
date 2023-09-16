#include "BSP_PWM.h"

// init structs
static GPIO_InitTypeDef gpio_struct;
static TIM_OC_InitTypeDef timer_oc_struct;
static TIM_Base_InitTypeDef timer_base_struct;

// https://visualgdb.com/tutorials/arm/stm32/timers/hal/

/**
 * @brief   Initialize all the GPIO pins meant for pwm
 * @param   None
 * @return  None
 */
void BSP_PWM_Init(void) {

    // enable timer 3, 8, 12 if they are disabled
    if (__HAL_RCC_TIM3_IS_CLK_DISABLED()) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    if (__HAL_RCC_TIM8_IS_CLK_DISABLED()) {
        __HAL_RCC_TIM8_CLK_ENABLE();
    }
    if (__HAL_RCC_TIM12_IS_CLK_DISABLED()) {
        __HAL_RCC_TIM12_CLK_ENABLE();
    }
    // enable port A/B/C clocks, if disabled
    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOB_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    // Configure timer pins by configuring the corresponding GPIO pins
    // Unfortunately HAL doesn't have a quick function to do GPIO_PinAFConfig
    gpio_struct.Pin = GPIO_PIN_0;
    gpio_struct.Alternate = GPIO_AF2_TIM3;
    gpio_struct.Mode = GPIO_MODE_AF_PP;
    gpio_struct.Speed = GPIO_SPEED_FAST;
    gpio_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &gpio_struct); // B0 = timer 3
    // C6 and C7 are timer 8
    gpio_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    gpio_struct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(GPIOC, &gpio_struct);
    // B14 and B15 are timer 12
    gpio_struct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
    gpio_struct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(GPIOB, &gpio_struct);

    // gpio stuff done, configure timer blocks themselves now
    timer_base_struct.Period = PWM_PERIOD;
    timer_base_struct.Prescaler = 0;
    timer_base_struct.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timer_base_struct.CounterMode = TIM_COUNTERMODE_UP;
    timer_base_struct.RepetitionCounter = 0;
    // struct to call the actual HAL timer config init
    TIM_HandleTypeDef timer_configuration;
    timer_configuration.Init = timer_base_struct;
    timer_configuration.Instance = TIM3;
    // timer 3
    HAL_TIM_Base_Init(&timer_configuration);
    timer_configuration.Instance = TIM8;
    // timer 8
    HAL_TIM_Base_init(&timer_configuration);
    timer_configuration.Instance = TIM12;
    // timer 12
    HAL_TIM_Base_init(&timer_configuration);

    // oc struct init
    
    // enable the timer blocks
    if (__HAL_RCC_TIM3_IS_CLK_DISABLED()) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    if (__HAL_RCC_TIM8_IS_CLK_DISABLED()) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    if (__HAL_RCC_TIM12_IS_CLK_DISABLED()) {
        __HAL_RCC_TIM12_CLK_ENABLE();
    }
    // set the config for the timers

    
}

/**
 * @brief   Sets a pin's duty cycle
 * @param   dutyCycle: duty cycle amount between 0 and the period
 * @param   pin: pin number whose speed should be changed
 * @return  ErrorStatus
 */
ErrorStatus BSP_PWM_Set(uint8_t pin, uint32_t speed) {

}

/**
 * @brief   Get current duty cycle of a single pin, return -1 if input is invalid
 * @param   pin Number
 * @return  Current PWM duty cycle of pin
 */
int BSP_PWM_Get(uint8_t pin) {

}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @note	THIS IS ALSO CODE THAT HAS NO HOME. You cannot get the state of ALL_CONTACTORS. As such, if that param is passed, it will return the state of the array contactor.
 * @param   Contactor to get state of
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_Get(uint8_t contactorChoice) {

}