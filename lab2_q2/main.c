
#include  <stdint.h>
#include  <stdbool.h>
#include  "inc/tm4c123gh6pm.h"
#include  "inc/hw_memmap.h"
#include  "inc/hw_types.h"
#include  "driverlib/sysctl.h"
#include  "driverlib/interrupt.h"
#include  "driverlib/gpio.h"
#include  "driverlib/timer.h"
#include "inc/hw_gpio.h"
uint32_t count=0;
uint32_t press=0;
int  main( void)

{
	uint32_t  ui32Period;

	SysCtlClockSet (SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;

	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	//GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure (TIMER0_BASE, TIMER_CFG_PERIODIC);

	//ui32Period = ( SysCtlClockGet () / 10) / 2;
	TimerLoadSet (TIMER0_BASE, TIMER_A, 160000);
	IntEnable (INT_TIMER0A);
	TimerIntEnable (TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

	TimerEnable(TIMER0_BASE, TIMER_A);
	GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
	SysCtlDelay(10000000);
	GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
	while (1)
	{
	}
}

void  Timer0IntHandler ( void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	// Read the current state of the GPIO pin and
	// write back the opposite state

	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0x0)
	{
		press++;
		if(press==0x2)
		{
			count++;

			//press=0;
		}
		else if( GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0x1)
		{
			press=0;
		}
		//GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	}
	else
	{
		press=0;
	}

}
