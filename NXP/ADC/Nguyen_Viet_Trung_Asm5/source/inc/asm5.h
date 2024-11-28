#ifndef _ASM5_H_
#define _ASM5_H_

/*******************************************************************************
* Include
*******************************************************************************/


/*******************************************************************************
* Define
*******************************************************************************/


#define GREEN_LED_PIN         (1 << 4)
#define RED_LED_PIN           (1 << 5)
#define BLUE_LED_PIN          (1 << 1)
#define FIVE_SECOND_LPIT      120000000
#define SYSTICK_0_00001_S     480
#define TEMP_SENSOR_CHANNEL   26

extern volatile uint8_t value_temp;
extern volatile int32_t time_delay;

/*******************************************************************************
* API
*******************************************************************************/
/**
  * @brief      initialize module LPIT for hardware trigger ADC
  * @retval     none
  */
void init_lpit();

/**
  * @brief      initialize led blue, led green, led red to use
  * @retval     None
  */
void init_led();

/**
  * @brief      initialize module ADC to check temperature
  * @retval     None
  */
void init_adc();

/**
  * @brief              change status of light green
  * @retval             None
  */
void change_led();


#endif /* _ASM5_H_ */
/*******************************************************************************
* EOF
*******************************************************************************/
