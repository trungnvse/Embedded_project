/*******************************************************************************
* Include
*******************************************************************************/
#include "MKE16Z4.h"
#include "asm3.h"

/*******************************************************************************
* Prototype
*******************************************************************************/

/*******************************************************************************
* Global variable
*******************************************************************************/



uint32_t g_second;
uint32_t g_two_thityfive;


/*******************************************************************************
* Code
*******************************************************************************/

void init_Led()
{
    /* Enable Clock for PORTB, PORTD */
	PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1);
	PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1);

    /* Setup PIND1, PINB4, PINB5 as GPIO */
    PORTD->PCR[1] |= PORT_PCR_MUX(1);
    PORTB->PCR[4] |= PORT_PCR_MUX(1);
    PORTB->PCR[5] |= PORT_PCR_MUX(1);

    /* Setup PORTD_PIN1, PORTB_PINB4, PINB5 as output */
    FGPIOB->PDDR |= GREEN_LED_PIN;
    FGPIOB->PDDR |= RED_LED_PIN;
    FGPIOD->PDDR |= BLUE_LED_PIN;

    /* Set default value for PORTD_PIN1, PORTB_PINB4, PINB5 */
    FGPIOB->PDOR |= GREEN_LED_PIN;
    FGPIOB->PDOR |= RED_LED_PIN;
    FGPIOD->PDOR |= BLUE_LED_PIN;
}

void init_timer()
{
	/* enable clock for PLIT module */
	/* default use Fast IRS 48MHZ */
	/* use option 3 ~ FIRC_CLK */
	PCC->CLKCFG[PCC_LPIT0_INDEX] |= PCC_CLKCFG_PCS(3);

	/* enable clock */
	PCC->CLKCFG[PCC_LPIT0_INDEX] |= PCC_CLKCFG_CGC_MASK;

	/* divide for 1 */
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);

	/* Reset the timer channels and registers */
    LPIT0->MCR |= LPIT_MCR_SW_RST_MASK;
    LPIT0->MCR &= ~LPIT_MCR_SW_RST_MASK;

    /* Setup timer operation in debug and doze modes and enable the module */
    LPIT0->MCR = LPIT_MCR_DBG_EN(0) |
				 LPIT_MCR_DOZE_EN(0) |
				 LPIT_MCR_M_CEN_MASK;

    /* Setup the channel counters operation mode to "32-bit Periodic Counter" */
    LPIT0->CHANNEL[0].TCTRL = LPIT_TCTRL_MODE(0);

    /* Set timer period for channel 0 as 0.05 second */
    LPIT0->CHANNEL[0].TVAL = (2400000);

    /* Enable channel0 interrupt */
	LPIT0->MIER |= LPIT_MIER_TIE0_MASK;

    NVIC_EnableIRQ(LPIT0_IRQn);

    /* Enable the channel timers*/
    LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0_MASK;

}

void LPIT0_IRQHandler(void)
{
	if((LPIT0->MSR & (LPIT_MSR_TIF0_MASK)))
	{
		LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* clear flat*/
		g_second--;
		g_two_thityfive--;
	}
}



/*******************************************************************************
* EOF
*******************************************************************************/
