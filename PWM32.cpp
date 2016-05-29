//	written by : Doug Reuter
//	PWM32 Servo32 Motor32 - Control up to 8 490hz motors or 50hz Servos (or a little of both) on 32bt arduino bords such as ( due or zero)
//	may work on other arduino boards as well
//	http://cheapogadget.com   https://github.com/cheapogadget/Taskmanager32
//	Copyright (C) 2016 Doug Reuter   dougreuter@gmail.com

//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.

//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.

//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Arduino.h"
#include "PWM32.h"
pwm32_type pmw32types[PWM32_TYPE_COUNT]{
#if defined(ARDUINO_SAM_DUE)
	{ pwm32_type_e::PWM32_TYPE_MOTOR_490hz, PWM_CMR_CPRE_CLKA , (490 * 2040),2040 },
	{ pwm32_type_e::PWM32_TYPE_SERVO_50hz, PWM_CMR_CPRE_CLKB , (50 * 20000),20000 }   ,
#elif defined( ARDUINO_SAMD_ZERO )
{ pwm32_type_e::PWM32_TYPE_MOTOR_490hz, 0 , 3,2040 },
{ pwm32_type_e::PWM32_TYPE_SERVO_50hz, 0 , 3,20000 },

#endif
	{ pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET, 0 , 0,0 }

};
#if defined( ARDUINO_SAMD_ZERO )
bool tcc_enabled[3] = { 0,0,0 };
#define MOTOR_ZERO_TCC_PER 2048 

#define TCC_CTRLA_DISABLE           (0x0u << TCC_CTRLA_ENABLE_Pos)
bool tcc_clock_enabled = false;

#endif
pwm32_t  pwm32[8] = {

#if defined( ARDUINO_SAMD_ZERO )
{ PWM32_D2_CH0 ,2u,0,1000,0, TCC0_CH0 ,0 }, // REG_TCC0_CCB0
{ PWM32_D3_CH1 ,3u,0,1000 ,0,TCC1_CH1 ,1 }, // REG_TCC1_CCB1
{ PWM32_D4_CH2 ,4u,0,1000 ,0,TCC1_CH0 ,1 }, // REG_TCC1_CCB0 
{ PWM32_D5_CH3 ,5u,0,1000 ,0,TCC0_CH1,0 }, // REG_TCC0_CCB1
{ PWM32_D6_CH4 , 6u,0,1000,0,TCC0_CH2,0 }, // REG_TCC0_CCB2
{ PWM32_D7_CH5 , 7u,0,1000,0,TCC0_CH3,0 }, // REG_TCC0_CCB3
{ PWM32_D11_CH6 , 11u,0,1000 ,0, TCC2_CH0,2 }, // REG_TCC2_CCB0
{ PWM32_D13_CH7 , 13u,0,1000 ,0,TCC2_CH1 , 2 }, // REG_TCC2_CCB1

#elif defined(ARDUINO_SAM_DUE)
	{ PWM32_D34_CH0 ,34u,0,1000 , 0 }, // Channel 0
	{ PWM32_D36_CH1 ,36u,0,1000, 0 }, // Channel 1
	{ PWM32_D38_CH2 ,38u,0,1000, 0 }, // Channel 2
	{ PWM32_D40_CH3 ,40u,0,1000, 0 }, // Channel 3
	{ PWM32_D9_CH4 , 9u,0,1000, 0 }, // Channel 4
	{ PWM32_D8_CH5 , 8u,0,1000, 0 }, // Channel 5
	{ PWM32_D7_CH6 , 7u,0,1000, 0 }, // Channel 6
	{ PWM32_D6_CH7 , 6u,0,1000 , 0 }, // Channel 7
#endif
};

PWM32 pwm32s[8]{
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET },
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET },
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET },
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET },
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET },
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET },
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET },
	{ 0, pwm32_type_e::PWM32_TYPE_MOTOR_NOT_SET }
};
bool pwm32ClockEnabled = false;
void pwm32_init() {

	if (!pwm32ClockEnabled)
	{
#if defined(ARDUINO_SAM_DUE)
		pmc_enable_periph_clk(PWM_INTERFACE_ID);
		PWMC_ConfigureClocks(pmw32types[PWM32_TYPE_MOTOR_490hz].clockSetting, pmw32types[PWM32_TYPE_SERVO_50hz].clockSetting, VARIANT_MCK);
		PMC->PMC_PCER1 = bit((ID_PWM - 32));
		PMC->PMC_PCER0 = bit(ID_PIOC);

		pwm32ClockEnabled = true;
#endif

	}

	for (int i = 7; i >= 0; i--)
		if (pwm32s[i].isSet == 1)
			if (pwm32s[i].pwm32P->pwmType->pwmType != pmw32types[PWM32_TYPE_MOTOR_NOT_SET].pwmType)
			{

#if defined( ARDUINO_SAMD_ZERO)
				//
				//	
				 
				if (pwm32s[i].pwm32P->tcc == 0)
				{
					REG_TCC0_CTRLA |=     // Divide GCLK4 by 1
						TCC_CTRLA_DISABLE;             // Enable the TCC0 output
					while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization)
				}
				else if (pwm32s[i].pwm32P->tcc == 1)
				{
					REG_TCC1_CTRLA |=     // Divide GCLK4 by 1
						TCC_CTRLA_DISABLE;             // Enable the TCC0 output
					while (TCC1->SYNCBUSY.bit.ENABLE);
				}
				else if (pwm32s[i].pwm32P->tcc == 2)
				{
					REG_TCC2_CTRLA |=     // Divide GCLK4 by 1
						TCC_CTRLA_DISABLE;             // Enable the TCC0 output
					while (TCC2->SYNCBUSY.bit.ENABLE);
				}

				if (tcc_clock_enabled == 0)
				{

					REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
						GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
					while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

					REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
						GCLK_GENCTRL_GENEN |         // Enable GCLK4
						GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
						GCLK_GENCTRL_ID(4);          // Select GCLK4
					while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
					tcc_clock_enabled = 1;
				}

				if (pwm32s[i].pwm32P->enabled == 0)
				{
					PORT->Group[g_APinDescription[pwm32s[i].pwm32P->digitalPin].ulPort].PINCFG[g_APinDescription[pwm32s[i].pwm32P->digitalPin].ulPin].bit.PMUXEN = 1;

					 
					if (pwm32s[i].pwm32P->digitalPin == 6 || pwm32s[i].pwm32P->digitalPin == 7)
						PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= pwm32s[i].pwm32P->port;// PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F; // PORT_PMUX_PMUXO_F;PORT_PMUX_PMUXE_F
					if (pwm32s[i].pwm32P->digitalPin == 2 || pwm32s[i].pwm32P->digitalPin == 5)
						PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= pwm32s[i].pwm32P->port;// PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F; // PORT_PMUX_PMUXO_F;PORT_PMUX_PMUXE_F
					if (pwm32s[i].pwm32P->digitalPin == 3 || pwm32s[i].pwm32P->digitalPin == 4)
						PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg |= pwm32s[i].pwm32P->port; // pwm32s[i].pwm32P->port;// PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F; // PORT_PMUX_PMUXO_F;PORT_PMUX_PMUXE_F
					if (pwm32s[i].pwm32P->digitalPin == 11 || pwm32s[i].pwm32P->digitalPin == 13)
						PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= pwm32s[i].pwm32P->port; // pwm32s[i].pwm32P->port;// PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F; // PORT_PMUX_PMUXO_F;PORT_PMUX_PMUXE_F

				}
				if ((tcc_enabled[0] + tcc_enabled[1]) == 0)// Feed GCLK4 to TCC0 and TCC1
				{
					REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TCC0 and TCC1
						GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
						GCLK_CLKCTRL_ID_TCC0_TCC1;   // Feed GCLK4 to TCC0 and TCC1
					while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
				}
				if (tcc_enabled[2] == 0)// Feed GCLK4 to TCC0 and TCC1
				{
					REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TCC0 and TCC1
						GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
						GCLK_CLKCTRL_ID_TCC2_TC3;   // Feed GCLK4 to TCC0 and TCC1
					while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
				}
				if (tcc_enabled[pwm32s[i].pwm32P->tcc] == 0)
				{
					if (pwm32s[i].pwm32P->tcc == 0)
					{
						// Dual slope PWM operation: timers countinuously count up to PER register value then down 0
						REG_TCC0_WAVE |= TCC_WAVE_POL(0xF) |         // Reverse the output polarity on all TCC0 outputs
							TCC_WAVE_WAVEGEN_DSBOTH;    // Setup dual slope PWM on TCC0
						while (TCC0->SYNCBUSY.bit.WAVE);               // Wait for synchronization

																	   // Each timer counts up to a maximum or TOP value set by the PER register,
						 					   // this determines the frequency of the PWM operation: 
						REG_TCC0_PER = pwm32s[i].pwm32P->pwmType->period; // 2040;         // Set the frequency of the PWM on TCC0 to 250kHz
						while (TCC0->SYNCBUSY.bit.PER);                // Wait for synchronization
					}
					else if (pwm32s[i].pwm32P->tcc == 1)
					{
						// Dual slope PWM operation: timers countinuously count up to PER register value then down 0
						REG_TCC1_WAVE |= TCC_WAVE_POL(0xF) |         // Reverse the output polarity on all TCC0 outputs
							TCC_WAVE_WAVEGEN_DSBOTH;    // Setup dual slope PWM on TCC0
						while (TCC1->SYNCBUSY.bit.WAVE);               // Wait for synchronization

																	   // Each timer counts up to a maximum or TOP value set by the PER register,
																	   // this determines the frequency of the PWM operation: 
						REG_TCC1_PER = pwm32s[i].pwm32P->pwmType->period;         // Set the frequency of the PWM on TCC0 to 250kHz
						while (TCC1->SYNCBUSY.bit.PER);                // Wait for synchronization
					}
					else if (pwm32s[i].pwm32P->tcc == 2)
					{
						// Dual slope PWM operation: timers countinuously count up to PER register value then down 0
						REG_TCC2_WAVE |= TCC_WAVE_POL(0xF) |         // Reverse the output polarity on all TCC0 outputs
							TCC_WAVE_WAVEGEN_DSBOTH;    // Setup dual slope PWM on TCC0
						while (TCC2->SYNCBUSY.bit.WAVE);               // Wait for synchronization

																	   // Each timer counts up to a maximum or TOP value set by the PER register,
																	   // this determines the frequency of the PWM operation: 
						REG_TCC2_PER = pwm32s[i].pwm32P->pwmType->period;         // Set the frequency of the PWM on TCC0 to 250kHz
						while (TCC2->SYNCBUSY.bit.PER);                // Wait for synchronization
					}
					tcc_enabled[pwm32s[i].pwm32P->tcc] = 1;
				}
			 

				switch (pwm32s[i].pwm32P->chan)
				{
				case  TCC0_CH0:
					REG_TCC0_CC0 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC3 - on D2
					while (TCC0->SYNCBUSY.bit.CC0);                // Wait for synchronization
					break;
				case  TCC0_CH1:
					REG_TCC0_CC1 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC1 - on D3
					while (TCC0->SYNCBUSY.bit.CC1);                // Wait for synchronization
					break;
				case  TCC0_CH2:
					REG_TCC0_CC2 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC3 - on D6
					while (TCC0->SYNCBUSY.bit.CC2);                // Wait for synchronization
					break;
				case  TCC0_CH3:
					REG_TCC0_CC3 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC3 - on D7
					while (TCC0->SYNCBUSY.bit.CC3);                // Wait for synchronization
					break;
				case  TCC1_CH0:
					REG_TCC1_CC0 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC3 - on D2
					while (TCC1->SYNCBUSY.bit.CC0);                // Wait for synchronization
					break;
				case  TCC1_CH1:
					REG_TCC1_CC1 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC1 - on D3
					while (TCC1->SYNCBUSY.bit.CC1);                // Wait for synchronization
					break;
				case  TCC2_CH0:
					REG_TCC2_CC0 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC3 - on D2
					while (TCC2->SYNCBUSY.bit.CC0);                // Wait for synchronization
					break;
				case  TCC2_CH1:
					REG_TCC2_CC1 = pwm32s[i].pwm32P->defaultMs;         // TCC0 CC1 - on D3
					while (TCC2->SYNCBUSY.bit.CC1);                // Wait for synchronization
					break;
				default:
					break;
				}


				if (pwm32s[i].pwm32P->tcc == 0)
				{				
					REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV8 |    // Divide GCLK4 by 1
						TCC_CTRLA_ENABLE;             // Enable the TCC0 output
					while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization)
					tcc_enabled[0] = 1;
				}
				else if (pwm32s[i].pwm32P->tcc == 1)
				{		
					REG_TCC1_CTRLA |= TCC_CTRLA_PRESCALER_DIV8 |    // Divide GCLK4 by 1
						TCC_CTRLA_ENABLE;             // Enable the TCC0 output
					while (TCC1->SYNCBUSY.bit.ENABLE);              // Wait for synchronization)
					tcc_enabled[1] = 1;  //Enable the TCC0 output
				}
				else if (pwm32s[i].pwm32P->tcc == 2)
				{				
					REG_TCC2_CTRLA |= TCC_CTRLA_PRESCALER_DIV8 |    // Divide GCLK4 by 1
						TCC_CTRLA_ENABLE;             // Enable the TCC0 output
					while (TCC2->SYNCBUSY.bit.ENABLE);              // Wait for synchronization)
					tcc_enabled[2] = 1;  //Enable the TCC0 output
				}


#elif defined(ARDUINO_SAM_DUE)
				PIOC->PIO_PDR |= pwm32[pwm32s[i].channel].port;
				PIOC->PIO_ABSR |= pwm32[pwm32s[i].channel].port;;
				PIOC->PIO_OER |= pwm32[pwm32s[i].channel].port;;
				PIOC->PIO_PUDR |= pwm32[pwm32s[i].channel].port;

				PWMC_ConfigureChannel(PWM_INTERFACE, pwm32s[i].channel, pwm32s[i].pwm32P->pwmType->clock, 0, 0);
				PWMC_SetPeriod(PWM_INTERFACE, pwm32s[i].channel, pwm32s[i].pwm32P->pwmType->period);
				PWMC_SetDutyCycle(PWM_INTERFACE, pwm32s[i].channel, pwm32s[i].pwm32P->defaultMs);
				PWMC_EnableChannel(PWM_INTERFACE, pwm32s[i].channel);
				pwm32s[i].pwm32P->enabled = true;
#endif
			}

}

PWM32::PWM32(uint32_t pin, pwm32_type_e pwmty)
{
	for (int a = 7; a >= 0; a--)

		if (pwm32[a].digitalPin == pin)
		{
			pwm32P = &pwm32[a];
			channel = a;

			for (int i = 0; i < PWM32_TYPE_COUNT; i++)
				if (pmw32types[i].pwmType == pwmty)
				{
					pwm32s[channel].channel = channel;
					pwm32P->pwmType = &pmw32types[i];
					pwm32s[channel].isSet = 1;
					pwm32s[channel].pwm32P = pwm32P;
				}
		}
}

void PWM32::set(uint16_t ms) 
{
	_v = ms;
#if defined(ARDUINO_SAMD_ZERO)
	switch (pwm32s[channel].pwm32P->chan)
	{
	case  TCC0_CH0:
		REG_TCC0_CCB0 = ms;         // TCC0 CC3 - on D2
		while (TCC0->SYNCBUSY.bit.CCB0);                // Wait for synchronization
		break;
	case  TCC0_CH1:
		REG_TCC0_CCB1 = ms;         // TCC0 CC1 - on D3
		while (TCC0->SYNCBUSY.bit.CCB1);                // Wait for synchronization
		break;
	case  TCC0_CH2:
		REG_TCC0_CCB2 = ms;         // TCC0 CC3 - on D6
		while (TCC0->SYNCBUSY.bit.CCB2);                // Wait for synchronization
		break;
	case  TCC0_CH3:
		REG_TCC0_CCB3 = ms;         // TCC0 CC3 - on D7
		while (TCC0->SYNCBUSY.bit.CCB3);                // Wait for synchronization
		break;
	case  TCC1_CH0:
		REG_TCC1_CCB0 = ms;         // TCC0 CC3 - on D2
		while (TCC1->SYNCBUSY.bit.CCB0);                // Wait for synchronization
		break;
	case  TCC1_CH1:
		REG_TCC1_CCB1 = ms;         // TCC0 CC1 - on D3
		while (TCC1->SYNCBUSY.bit.CCB1);                // Wait for synchronization
		break;
	case  TCC2_CH0:
		REG_TCC2_CCB0 = ms;         // TCC0 CC3 - on D2
		while (TCC2->SYNCBUSY.bit.CCB0);                // Wait for synchronization
		break;
	case  TCC2_CH1:
		REG_TCC2_CCB1 = ms;         // TCC0 CC1 - on D3
		while (TCC2->SYNCBUSY.bit.CCB1);                // Wait for synchronization
		break;
	default:
		break;
}
	#elif defined(ARDUINO_SAM_DUE)
	if ((PWM->PWM_SR & bit(channel)) == 0)
		PWM->PWM_CH_NUM[channel].PWM_CDTY = ms;
	else
		PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = ms;
#endif
}
