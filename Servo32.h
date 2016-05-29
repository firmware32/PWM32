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
// Servo32.h

#ifndef _SERVO32_h
#define _SERVO32_h


 
	#include "arduino.h"
#include "PWM32.h"

class Servo32 : PWM32
{
 protected:


 public:
	 Servo32(uint8_t pin);
	 void set(uint16_t ms);
	 uint16_t get();
};



#endif

