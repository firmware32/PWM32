# PWM32

PWM32 Servo32 Motor32 - Control up to 8 490hz motors or 50hz Servos (or a little of both) on 32bt arduino bords such as ( due or zero)



```
//Easy to use:  works with Arduino DUE or Arduino Zero (SAMD21 boards)
#include <Servo32.h>
#include <Motor32.h>

Servo32 servo1(3);
Motor32 motor1(2);

void setup(){
pwm32_init(); // Call this once.
}

void loop(){
servo1.set(1500); // 50hz
motor1.set(1000); // 490hz
}
// thats it :)
	
```
