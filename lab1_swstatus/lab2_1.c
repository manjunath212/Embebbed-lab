#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

#define PWM_FREQUENCY 55
uint32_t time=56666;
void check_button();
int main(void)
{
	volatile uint32_t ui32Load;
	volatile uint32_t ui32PWMClock;
	volatile uint8_t ui8Adjust;
	ui8Adjust=83;
	uint32_t i;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	//SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_1);
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_2);
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_3);
	GPIOPinConfigure(GPIO_PF1_M1PWM5);
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinConfigure(GPIO_PF3_M1PWM7);

	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK)=GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR)|=0x01;
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK)=0;
	GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0,GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

	ui32PWMClock=SysCtlClockGet()/64;
	ui32Load=(ui32PWMClock/PWM_FREQUENCY)-1;
	PWMGenConfigure(PWM1_BASE,PWM_GEN_2,PWM_GEN_MODE_DOWN);
	PWMGenConfigure(PWM1_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE,PWM_GEN_2,ui32Load);
	PWMGenPeriodSet(PWM1_BASE,PWM_GEN_3,ui32Load);

	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_5,ui8Adjust*ui32Load/1000);
	PWMOutputState(PWM1_BASE,PWM_OUT_5_BIT,true);
	PWMGenEnable(PWM1_BASE,PWM_GEN_2);

	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,ui8Adjust*ui32Load/1000);
	PWMOutputState(PWM1_BASE,PWM_OUT_6_BIT,true);
	//PWMGenEnable(PWM1_BASE,PWM_GEN_2);

	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,ui8Adjust*ui32Load/1000);
	PWMOutputState(PWM1_BASE,PWM_OUT_7_BIT,true);
	PWMGenEnable(PWM1_BASE,PWM_GEN_3);

	for(i=0;i<240;i++)
		{
			ui8Adjust=10+i;
			PWMPulseWidthSet(PWM1_BASE,PWM_OUT_5,ui8Adjust*ui32Load/1000);

			SysCtlDelay(56666);
		}
	while(1)
	{
		/*//if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
		{
		//ui8Adjust--;
		//if(ui8Adjust<56)
		{
		ui8Adjust=56;
		}
		PWMPulseWidthSet(PWM1_BASE,PWM_OUT_5,ui8Adjust*ui32Load/1000);
		}
		SysCtlDelay(100000);
		//if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
		{
		//ui8Adjust++;
		//if(ui8Adjust>111)
		{
		ui8Adjust=111;
		}
		PWMPulseWidthSet(PWM1_BASE,PWM_OUT_5,ui8Adjust*ui32Load/1000);
		}
		SysCtlDelay(100000);*/
		 //Blue brightness goes up

		for(i=0;i<240;i++)
			{
				ui8Adjust=10+i;
				PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,ui8Adjust*ui32Load/1000);
				check_button();
				SysCtlDelay(time);
			}
		//Red brightness goes down
		for(i=0;i<240;i++)
		{
			ui8Adjust=250-i;
			PWMPulseWidthSet(PWM1_BASE,PWM_OUT_5,ui8Adjust*ui32Load/1000);
			check_button();
			SysCtlDelay(time);
		}

		//Green brightness goes up
		for(i=0;i<240;i++)
			{
				ui8Adjust=10+i;
				PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,ui8Adjust*ui32Load/1000);
				check_button();
				SysCtlDelay(time);
			}
		//Blue brightness goes down
		for(i=0;i<240;i++)
		{
			ui8Adjust=250-i;
			PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,ui8Adjust*ui32Load/1000);
			check_button();
			SysCtlDelay(time);
		}
	    //Red brightness goes up
		for(i=0;i<240;i++)
			{
				ui8Adjust=10+i;
				PWMPulseWidthSet(PWM1_BASE,PWM_OUT_5,ui8Adjust*ui32Load/1000);
				check_button();
				SysCtlDelay(time);
			}
		//Green brightness goes down
		for(i=0;i<240;i++)
		{
			ui8Adjust=250-i;
			PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,ui8Adjust*ui32Load/1000);
			check_button();
			SysCtlDelay(time);
		}
	}
}

void check_button()
{
	if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00))
	{
		if(time<200000)
		{
			time=time+100000;
		}
	}
	if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00))
	{
		if(time>5666)
		{
			time=time-50000;
		}
	}
}



