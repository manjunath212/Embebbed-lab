#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include  "inc/hw_gpio.h"
#include  "driverlib/adc.h"
#include "inc/hw_gpio.h"
#include  "inc/tm4c123gh6pm.h"
#include  "driverlib/gpio.h"

int   main( void)
{
	uint32_t ui32ADC0Value[4],ui32ADC1Value[4];
	volatile uint32_t ui32TempAvg,ui32TempAvg2;
	volatile uint32_t ui32TempValueC;
	volatile uint32_t ui32TempValueF;

	SysCtlClockSet (SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 );
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);

	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH7);
	ADCSequenceStepConfigure (ADC0_BASE, 1, 1, ADC_CTL_CH7);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH7);
	ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 1);
	ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH6);
	ADCSequenceStepConfigure (ADC1_BASE, 1, 1, ADC_CTL_CH6);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH6);
	ADCSequenceStepConfigure(ADC1_BASE,1,3,ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 1);
	while (1)
	{
		ADCIntClear(ADC0_BASE, 1);
		ADCIntClear(ADC1_BASE, 1);
		ADCProcessorTrigger(ADC0_BASE, 1);
		ADCProcessorTrigger(ADC1_BASE, 1);

		while (!(ADCIntStatus (ADC0_BASE, 1, false) && ADCIntStatus (ADC1_BASE, 1, false)))
		{
		}

		ADCSequenceDataGet (ADC0_BASE, 1, ui32ADC0Value);
		ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
		ui32TempValueC = (1475  -  ((2475 * ui32TempAvg)) / 4096)/10;
		ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
		ADCSequenceDataGet (ADC1_BASE, 1, ui32ADC1Value);
		ui32TempAvg2 = (ui32ADC1Value[0] + ui32ADC1Value[1] + ui32ADC1Value[2] + ui32ADC1Value[3] + 2)/4;
	}
}
