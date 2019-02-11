#include <ISL29125_SoftWire.h>

/******************************************************************************
ISL29125_basics.ino
Simple example for using the ISL29125 RGB sensor library.
Originally by Jordan McConnell @ SparkFun Electronics, modified by Christos Koutsouradis
28 Oct 2016
https://github.com/christosku/ISL29125_SoftWire

This example declares an SFE_ISL29125 object called RGB_sensor. The 
object/sensor is initialized with a basic configuration so that it continuously
samples the light intensity of red, green and blue spectrums. These values are
read from the sensor every 2 seconds and printed to the Serial monitor.

Developed/Tested with:
Arduino Uno
Arduino IDE 1.6.12

Requires:
ISL29125_SoftWire Library (https://github.com/christosku/ISL29125_SoftWire)
SoftwareWire Library (https://github.com/Fire7/SoftwareWire)


This code is beerware.
Distributed as-is; no warranty is given. 
******************************************************************************/

// Declare sensor object
ISL29125_SOFT RGB_sensor[2];

void setup()
{
  RGB_sensor[0].init(2,3);
  RGB_sensor[1].init(4,5); 
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the ISL29125 with simple configuration so it starts sampling (Pin 2 is SDA, pin 3 is SCL)
  if (RGB_sensor[0].init(2,3)) 
  {
    Serial.println("Sensor_1 Initialization Successful\n\r");
  }
  else{
    Serial.println("Sensor_1 Initialization Failed\n");
  }
  // Initialize the ISL29125 with simple configuration so it starts sampling (Pin 2 is SDA, pin 3 is SCL)
  if (RGB_sensor[1].init(4,5)) 
  {
    Serial.println("Sensor_2 Initialization Successful\n\r");
  }
  else{
    Serial.println("Sensor_2 Initialization Failed\n");
  }
}

// Read sensor values for each color and print them to serial monitor
void loop()
{
  // Read sensor_1 values (16 bit integers)
  unsigned int red_1 = RGB_sensor[0].readRed();
  unsigned int green_1 = RGB_sensor[0].readGreen();
  unsigned int blue_1 = RGB_sensor[0].readBlue();

  // Read sensor_2 values (16 bit integers)
  unsigned int red_2 = RGB_sensor[1].readRed();
  unsigned int green_2 = RGB_sensor[1].readGreen();
  unsigned int blue_2 = RGB_sensor[1].readBlue();
  
  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("Sensor 1 Red: "); Serial.println(red_1,HEX);
  Serial.print("Sensor 1 Green: "); Serial.println(green_1,HEX);
  Serial.print("Sensor 1 Blue: "); Serial.println(blue_1,HEX);
  Serial.println();
  Serial.print("Sensor 2 Red: "); Serial.println(red_2,HEX);
  Serial.print("Sensor 2 Green: "); Serial.println(green_2,HEX);
  Serial.print("Sensor 2 Blue: "); Serial.println(blue_2,HEX);
  Serial.println();
  delay(2000);
}
