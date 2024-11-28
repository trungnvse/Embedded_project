/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include "MKE16Z4.h"
#include "asm4.h"
/*******************************************************************************
* Prototype
*******************************************************************************/

/*******************************************************************************
* Global variable
*******************************************************************************/

uint32_t led_blue_on;
uint32_t led_blue_off;
uint32_t led_green_on;
uint32_t led_green_off;

/*******************************************************************************
* Code
*******************************************************************************/

uint32_t get_timer()
{
	/* time set milliseconds */
	return (RTC->TSR - 1)*1024 + ((RTC->TPR)>>5);
}

void change_status_light_green(const uint32_t timer)
{
	uint32_t temp_green = 0;
	temp_green = timer % ((led_green_on + led_green_off) * 1000);
	if(temp_green < (led_green_on * 1000))
	{
		FGPIOB->PDOR &= ~GREEN_LED_PIN;
	}else{
		FGPIOB->PDOR |= GREEN_LED_PIN;
	}
}

void change_status_light_blue(const uint32_t timer)
{
	uint32_t temp_blue  = 0;
	temp_blue  = timer % ((led_blue_on + led_blue_off) * 1000);
	if(temp_blue < (led_blue_on * 1000))
	{
		FGPIOD->PDOR &= ~BLUE_LED_PIN;
	}else{
		FGPIOD->PDOR |= BLUE_LED_PIN;
	}
}

void init_rstc()
{
	/* choose clock LPO 1 kHZ clock for counter RTC */
    /* enable clock for register */
    PCC->CLKCFG[PCC_RTC_INDEX] |=  PCC_CLKCFG_CGC(1);

    /* configure RTC*/
	while (RTC->SR & RTC_SR_TIF_MASK)
	{
		/* reset module*/
		RTC->CR |= RTC_CR_SWR_MASK;
		RTC->CR &= ~RTC_CR_SWR_MASK;
		RTC->TSR = 1;/* clear flat TIF */
	}
	/* select LPO for counter*/
	RTC->CR |= (1<<7);
    /* disable interrupt */
	RTC->IER = 0;

    /* disable timer */
	RTC->SR &= ~RTC_SR_TCE_MASK;
    /* set second register */
	RTC->TPR = 32;
	RTC->TSR = 1;
	/* enable timer */
	RTC->SR |= RTC_SR_TCE_MASK;
}

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

/*******************************************************************************
* EOF
*******************************************************************************/
