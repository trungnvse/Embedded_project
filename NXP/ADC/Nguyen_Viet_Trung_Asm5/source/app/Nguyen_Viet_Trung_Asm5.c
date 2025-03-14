/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include "MKE16Z4.h"
#include "fsl_debug_console.h"
#include "asm5.h"

/*******************************************************************************
* Code
*******************************************************************************/
int main(void) {
	SysTick_Config(SYSTICK_0_00001_S);  /* 0.00001s ~ 480 */
	init_adc();
	init_lpit();
	init_led();
	value_temp = 25;

    while(1) {
    	change_led();
    }
    return 0 ;
}
/*******************************************************************************
* EOF
*******************************************************************************/
