#ifndef _ASM3_H_
#define _ASM3_H_

/*******************************************************************************
* Include
*******************************************************************************/


/*******************************************************************************
* Define
*******************************************************************************/


#define GREEN_LED_PIN         (1 << 4)
#define RED_LED_PIN           (1 << 5)
#define BLUE_LED_PIN          (1 << 1)



extern uint32_t g_second;
extern uint32_t g_two_thityfive;

/*******************************************************************************
* API
*******************************************************************************/

/**
  * @brief      initialize led blue, led green, led red to use
  * @retval     None
  */
void init_Led();

/**
  * @brief      initialize timer LPIT to use interrupt after 0.05s
  * @retval     None
  */
void init_timer();

/**
  * @brief      handle when occurs, count down two global variable and clear flat
  * @retval     None
  */
void LPIT0_IRQHandler(void);

#endif /* _ASM3_H_ */
/*******************************************************************************
* EOF
*******************************************************************************/
