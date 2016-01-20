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

#include <Servo32.h>
#include <Motor32.h>


uint32_t onRise;
uint32_t pulseWidth;
uint32_t ctt;
void pwmTime() { if (digitalRead(10)) onRise = micros(); else pulseWidth = micros() - onRise; ctt++; }
#if defined(ARDUINO_SAMD_ZERO)
 

#define SER SerialUSB

	//TCCO
	Servo32 servo1(3);		// On Arduino Zero 3,4  (TCC0) must be the same type (servo or motor)
	Servo32 servo2(4);		 

	//TCC1
	Motor32 motor1(2);		// On Arduino Zero 2,5,6,7  (TCC1) must be the same type (servo or motor)
	Motor32 motor2(5);		 
	Motor32 motor3(6);		 
	Motor32 motor4(7);		 

	//TCC2
	Servo32 servo3(11);		//	On Arduino Zero 11,13  (TCC2) must be the same type (servo or motor)
	Servo32 servo4(13);

	
 

#elif defined(ARDUINO_SAM_DUE)

	#define SER Serial

	Servo32 servo1(9);		// On Arduino Due 9,8,7,6, 40,38,36,34 can be either ( servo or motor)
	Motor32 motor1(7);		 
	Servo32 servo2(6);
	Motor32 motor2(5); 
	Servo32 servo3(34);		// On Arduino Due 9,8,7,6, 40,38,36,34 can be either ( servo or motor)
	Motor32 motor3(36);
	Servo32 servo4(38);
	Motor32 motor4(40);
 
 
#endif

// Instructions hook wire between inputs    (Zero D10 or due D10 ) and any configured PWM (servo or motor) to see the measured output
// D10 - 3   D10 - 4 ... DUE D10 - D0 D10 - D8..etc
void setup()
{
	SER.begin(115200);
	while (!SER);		//Wait for serial to start
 
	// Some interupts to grab some measurements;
	pinMode(10, INPUT_PULLUP); attachInterrupt(10, pwmTime, CHANGE);
 
	// v IMPORTANT MUST BE CALLED ONCE
	pwm32_init();
	// ^IMPORTANT MUST BE CALLED ONCE 
		servo1.set(1100);
		servo2.set(1200);
		servo3.set(1300);
		servo4.set(1400);
		motor1.set(1500);
		motor2.set(1600);
		motor3.set(1700);
		motor4.set(1800);
 
}

void loop()
{
	// timing
 
	SER.print("\tPW ");    SER.print(pulseWidth); SER.print("\t");
	uint32_t cc = ctt;
	delay(1000);

	SER.print("\tFQ ");   SER.print(" "); SER.print((ctt - cc) >> 1); SER.println();
 

}
