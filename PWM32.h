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

// PWM32.h

#ifndef _PWM32_h
#define _PWM32_h


#include "arduino.h"

#if defined( ARDUINO_SAMD_ZERO )
#define PWM32_D2_CH0			 PORT_PMUX_PMUXE_F
#define PWM32_D3_CH1			PORT_PMUX_PMUXO_F
#define PWM32_D4_CH2         PORT_PMUX_PMUXE_F 
#define PWM32_D5_CH3         PORT_PMUX_PMUXO_F
#define PWM32_D6_CH4         PORT_PMUX_PMUXE_F
#define	PWM32_D7_CH5			PORT_PMUX_PMUXO_F 
#define PWM32_D11_CH6          PORT_PMUX_PMUXE_E  
#define PWM32_D13_CH7        PORT_PMUX_PMUXO_E  


extern bool tcc_enabled[3];
enum pwm32_pins_e {
	PWM32_PIN_D2_1 = 2,
	PWM32_PIN_D3_1 = 3,
	PWM32_PIN_D4_2 = 4,
	PWM32_PIN_D5_2 = 5,
	PWM32_PIN_D6_2 = 6,
	PWM32_PIN_D7_2 = 7,
	PW32_PIN_D11_3 = 11,
	PWM32_PIN_D13_3 = 13

};
#elif defined(ARDUINO_SAM_DUE)
#define PWM32_D34_CH0			(1u << 2) 
#define PWM32_D36_CH1			(1u << 4) 
#define PWM32_D38_CH2         (1u << 6) 
#define PWM32_D40_CH3         (1u << 8) 
#define PWM32_D9_CH4          (1u << 21) /**PIO_PC21*/
#define	PWM32_D8_CH5			(1u << 22) /**PIO_PC22*/
#define PWM32_D7_CH6          (1u << 23) /**PIO_PC23 */
#define PWM32_D6_CH7          (1u << 24) /**PIO_PC24 */
#endif

enum pwm32_type_e {

	PWM32_TYPE_MOTOR_490hz,
	PWM32_TYPE_SERVO_50hz,
	PWM32_TYPE_MOTOR_NOT_SET,
	PWM32_TYPE_COUNT
};
typedef struct {
	pwm32_type_e pwmType;
	uint32_t  clock;	//PWM_CMR_CPRE_CLKA
	uint32_t  clockSetting;	///(490 * 2000)
	uint32_t period;


} pwm32_type;
extern pwm32_type pmw32types[PWM32_TYPE_COUNT];
typedef struct {
	uint32_t port;
	uint32_t digitalPin;
	bool enabled;
	uint16_t defaultMs;
	uint32_t period;

#if defined( ARDUINO_SAMD_ZERO )
	uint32_t chan;
	uint8_t tcc;
#endif
	pwm32_type * pwmType;
} pwm32_t;
extern pwm32_t  pwm32[8];
#if defined( ARDUINO_SAMD_ZERO )

#endif
void pwm32_init();


class PWM32
{
protected:
	uint16_t _v;
public:

	bool isSet;
	uint8_t channel;
	pwm32_t * pwm32P;

	void set(uint16_t ms);
	uint16_t get();
	PWM32(uint32_t pin, pwm32_type_e pwmtype);
};
#endif

