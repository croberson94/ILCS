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
  
  //LEDS.I2CSAVEPARAM(0x15E);
}

void loop() {
  LEDS.I2CWRITE2BYTES(0x15E,WALKTIME,0xC3);  // Set walk time to 2 seconds
  LEDS.I2CWRITE2BYTES(0x15E,FADERATE,0x6E);
    // Set the brightness
  LEDS.I2CWRITE6BYTES(0x15E,INTENSITY_RGB, 0x566,0x555,0x555);
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0x19A); // 10% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0x555, 0x555, 0x555);
  //delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0x332); // 20% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  //delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0x4CC); // 30% Brightness
  LEDS.I2CWRITE6BYTES(0x15E,INTENSITY_RGB, 0x55F,0x555,0x555);
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  //delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0x665); // 40% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  //delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0x7FF); // 50% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  //delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0x998); // 60% Brightness
  delay(4000); // wait 4 seconds
  LEDS.I2CWRITE6BYTES(0x15E,INTENSITY_RGB, 0x555,0x555,0x55F);
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0xB31); // 70% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  //delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0xCCB); // 80% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  delay(4000); // wait 4 seconds
  LEDS.I2CWRITE6BYTES(0x15E,INTENSITY_RGB, 0x555,0x555,0x566);
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0xE64); // 90% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  //delay(4000); // wait 4 seconds
  LEDS.I2CWRITE2BYTES(0x15E,DIMMINGLEVEL,0xFFF); // 100% Brightness
  delay(4000); // wait 4 seconds
  //LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
  //delay(4000); // wait 4 seconds
	
 
//	LEDS.SetIntensityRGB(0xFFF, 0xFFF, 0xFFF);
//	delay(4000); // wait 4 seconds
//  //LEDS.SetColor(White);
//  delay(4000); // wait 4 seconds
}
