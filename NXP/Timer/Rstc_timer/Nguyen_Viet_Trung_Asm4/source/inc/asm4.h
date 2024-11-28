#ifndef _ASM4_H_
#define _ASM4_H_

/*******************************************************************************
* Include
*******************************************************************************/


/*******************************************************************************
* Define
*******************************************************************************/


#define GREEN_LED_PIN         (1 << 4)
#define RED_LED_PIN           (1 << 5)
#define BLUE_LED_PIN          (1 << 1)


extern uint32_t led_blue_on;
extern uint32_t led_blue_off;
extern uint32_t led_green_on;
extern uint32_t led_green_off;


/*******************************************************************************
* API
*******************************************************************************/

/**
  * @brief      get time from TSR register and TPR register, time set milliseconds
  * @retval     return real time from application start
  */
uint32_t get_timer();

/**
  * @brief      initialize led blue, led green, led red to use
  * @retval     None
  */
void init_Led();

/**
  * @brief      initialize timer RSTC
  * @retval     None
  */
void init_rstc();

/**
  * @brief              change status of light blue
  * uint32_t timer[in]  real time from application start
  * @retval             None
  */
void change_status_light_blue(const uint32_t timer);

/**
  * @brief              change status of light green
  * uint32_t timer[in]  real time from application start
  * @retval             None
  */
void change_status_light_green(const uint32_t timer);


#endif /* _ASM4_H_ */
/*******************************************************************************
* EOF
*******************************************************************************/
