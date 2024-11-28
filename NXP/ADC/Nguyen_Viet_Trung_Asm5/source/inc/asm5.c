/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include "MKE16Z4.h"
#include "fsl_debug_console.h"
#include "asm5.h"

/*******************************************************************************
* Global variable
*******************************************************************************/
volatile uint8_t value_temp;
volatile int32_t time_delay;

/*******************************************************************************
* Code
*******************************************************************************/

/* convert value of RA -> degree
 * follow AN3031 document, For VDD = 3 V, the readings range from 277d at –40°C down to 176d at 130°C
 * so, we will take RA decrease 1 ~ 1.7 degree
 * 615  ~   25 degree
 * 605  ~   42 degree
 * ...
 * 600  ~   50 degree
 * */

/* interrupt after 5s */
void ADC0_IRQHandler(void)
{
	uint32_t result = 0;
	result = ADC0->R[0];
    value_temp = (615 - result) * 1.7 + 25;
    PRINTF("value of RA: %d\n", result);
    PRINTF("degree     : %d\n", value_temp);
}

void init_adc()
{
	/* enable clock for ADC module */
	/* set clock for register is 24mhz*/
	/* set clock for ALTCLK1 is 12mhz*/

    /* enable clock for register*/
	PCC->CLKCFG[PCC_ADC0_INDEX] &= ~PCC_CLKCFG_CGC_MASK;
	PCC->CLKCFG[PCC_ADC0_INDEX] |= PCC_CLKCFG_PCS(3);
	PCC->CLKCFG[PCC_ADC0_INDEX] |= PCC_CLKCFG_CGC_MASK;

    /* divide by 2*/
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(2);/* divide clock 48mhz by 2 = 24mhz*/

	/* divide clock 24mhz by 2, now clock for ALTCLK1 is 12mhz */
	ADC0->CFG1 |= ADC_CFG1_ADIV(1);

#if 1
	/* Calibration */
	ADC0->CLPX = 0;
	ADC0->CLP0 = 0;
	ADC0->CLP1 = 0;
	ADC0->CLP2 = 0;
	ADC0->CLP3 = 0;
	ADC0->CLP9 = 0;
	ADC0->CLPS = 0;

	/* Hardware average function enabled */
	ADC0->SC3 |= ADC_SC3_AVGE(1);
	/* set 32 ADC conversions will be averaged*/
	ADC0->SC3 |= ADC_SC3_AVGS(3);
	/* enable Calibration */
	ADC0->SC3 |= ADC_SC3_CAL(1) ;

	/* wait for Calibration to finish -> COCO is 1 */
	while(!(ADC0->SC1[0] & (1<<7)))
	{}

	/* Hardware average function disabled */
	ADC0->SC3 |= ADC_SC3_AVGE(0);
	/* set 0 ADC conversions will be averaged*/
	ADC0->SC3 |= ADC_SC3_AVGS(0);
	/* set one conversions */
	ADC0->SC3 |= ADC_SC3_ADCO(0);

#endif

    /* configuration for ADC module */
	/* select mode 12bit*/
	ADC0->CFG1 |= ADC_CFG1_MODE(1);

    /* select input clock by ALTCLK1 */
	ADC0->CFG1 |= ADC_CFG1_ADICLK(0);

	/* divide clock 48mhz by 2 */
	ADC0->CFG1 |= ADC_CFG1_ADIV(1);

	/* select sampling timer*/
	ADC0->CFG2 = 12;

	/* select default reference voltage , use 5v*/
	ADC0->SC2 |= ADC_SC2_REFSEL(0);

	/* select hardware trigger */
	ADC0->SC2 |= ADC_SC2_ADTRG(1);

	/* clear SC1 register */
	ADC0->SC1[0] &= ~31;
	/* select channel 26 ~ temperature sensor */
	ADC0->SC1[0] |= ADC_SC1_ADCH(TEMP_SENSOR_CHANNEL);

	/* enable interrupt */
	ADC0->SC1[0] |= ADC_SC1_AIEN(1);
	NVIC_EnableIRQ(ADC0_IRQn);
}

void init_lpit()
{
	/* enable clock for LPIT */
	PCC->CLKCFG[PCC_LPIT0_INDEX] &= ~PCC_CLKCFG_CGC_MASK;
	PCC->CLKCFG[PCC_LPIT0_INDEX] |= PCC_CLKCFG_PCS(3);
	PCC->CLKCFG[PCC_LPIT0_INDEX] |= PCC_CLKCFG_CGC_MASK;

	/* divide by 2*/
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(2);

	/* configuration for LPIT */
	/* reset LPIT */
	LPIT0->MCR |= LPIT_MCR_SW_RST_MASK;
	LPIT0->MCR &= ~LPIT_MCR_SW_RST_MASK;

	/* Setup timer operation in debug and doze modes and enable LPIT module */
	LPIT0->MCR = LPIT_MCR_DBG_EN(1) |
				 LPIT_MCR_DOZE_EN(1) |
				 LPIT_MCR_M_CEN_MASK;

	/* use mode 32-bit Periodic Counter */
	LPIT0->CHANNEL[0].TCTRL = LPIT_TCTRL_MODE(0);

	/* configuration 5s */
	LPIT0->CHANNEL[0].TVAL = (FIVE_SECOND_LPIT - 1);

	/* Set Timer 0,1 Enable */
	LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0_MASK | LPIT_SETTEN_SET_T_EN_1_MASK;

	/* Selects trigger source for ADC0 is TRGMUX*/
	SIM->ADCOPT |= SIM_ADCOPT_ADC0TRGSEL(1)|SIM_ADCOPT_ADC0PRETRGSEL(1);

	/* select TRGMUX output as ADC pre-trigger and trigger source */
	TRGMUX0->TRGCFG[TRGMUX_ADC0_INDEX] = TRGMUX_TRGCFG_SEL0(7) | TRGMUX_TRGCFG_SEL1(8) ;
}

void init_led()
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

void change_led()
{
	/* because it can only be heated at a certain temperature range (if not, controller will be destroyed),
	 * So, range will be from 10 ~ 60 degree */
	/* 10  degree ~ on/off = 0/100 */
	/* increase 1 degree ~ increase on 2 */
	/* 25  degree ~ on/off = 30/70 */
	/* 30  degree ~ on/off = 40/60 */
	/* 35  degree ~ on/off = 50/50 */
	/* 50  degree ~ on/off = 80/20 */
	/* 60  degree ~ on/off = 100/0 */
	time_delay = (value_temp - 10) * 2 ;
	FGPIOB->PDOR &= ~GREEN_LED_PIN;
	while(0 != time_delay)
	{

	}

	time_delay = (value_temp - 10) * 2 ;
	FGPIOB->PDOR |= GREEN_LED_PIN;
	while(100 != (100 - time_delay))
	{

	}
}

void SysTick_Handler(void)
{
	/* 0.00001s */
	if( time_delay > 0)
	{
		time_delay--;
	}
}
/*******************************************************************************
* EOF
*******************************************************************************/
