/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
//#include "tm4c123gh6pm.h"
/*uint8_t ui8PinData=2;
uint8_t sw2Status=0;*/
void ButtonsInit(void);
int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//unlocking port F Pin 0
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;

	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
	uint8_t  count=0;
    //
    // Configure the GPIO port for the LED operation.
    //
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	//GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	//GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	while(1)
	{
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==1)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
		}
		else
		{
			count++;
			while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0)
			{
				if(count%3==1)
				{
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,2);
				}
				else if(count%3==2)
				{
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,4);
				}
				else
				{
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,8);
				}
			}
		}
		/*if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==1)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x2);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x8);
		}*/
		/*if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0)
		{
			sw2Status++;
			while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0);
		}*/
	}
	return 0;
}

