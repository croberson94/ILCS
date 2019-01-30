/*!
*	\file		Strip Test.ino
*	\author 	Inderpreet Singh (google.com/+InderpreetSingh)
* 	\license	GPL2(see license.txt)
*
*	\section 	Description
				Example Code to test if everything if OK. If all iz well then the
				strip should cycle through the three basic colors
*
*	\section	HISTORY
*	v1.0
*/
// ----------------------------------------------------------------------------
#include <RGBLEDLighting.h>
#include <Wire.h>

InfineonRGB LEDS;	// Create Object

void setup() {
  // put your setup code here, to run once:
  LEDS.begin();		// Cycle and set everything to zero.
  LEDS.SetDimmingLevel(0x07FF);	// Maximum Brightness
  //LEDS.CHANGEADDRESS(0x15E,0x15A); // Change address to 0x15A
  //LEDS.I2CSAVEPARAM(0x15A);
}

void loop() {

	
	
	// Set the brightness
	LEDS.SetDimmingLevel(0x07FF);	// 50% Brightness
	delay(1000);
	
	LEDS.SetColor(White);
    delay(2000);
	
    LEDS.SetColor(Maroon);
	delay(2000);
}
