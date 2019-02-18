/*
 * The University of Texas at Tyler
 * Intellegent Lighting Control System Team
 * 
 * Purpose:       This script will allow us to test the Master/Slave 
 *                communication between two arduino boards
 *                
 * Author:        Griggs Hutaff  
 *
 * Other Credits:  Adapted from (https://www.arduino.cc/en/Tutorial/MasterReader)
 *					I2C_Anything library from (https://github.com/nickgammon/I2C_Anything)
 * 
 * Last Revision: 02/13/2019
 * 
 */

#include <Wire.h>

// declare intensity values for all 4 sectors, source: DRIVER
unsigned int intensity_red_s1, intensity_green_s1, intensity_blue_s1;
unsigned int intensity_red_s2, intensity_green_s2, intensity_blue_s2;
unsigned int intensity_red_s3, intensity_green_s3, intensity_blue_s3;
unsigned int intensity_red_s4, intensity_green_s4, intensity_blue_s4;

//unsigned int current_red, current_green, current_blue; NOT SURE IF WE WILL NEED THIS VALUE

// declare color sensor values for all 4 sectors, source: RGB Sensor
unsigned int sensor_red_s1, sensor_green_s1, sensor_blue_s1;
unsigned int sensor_red_s2, sensor_green_s2, sensor_blue_s2;
unsigned int sensor_red_s3, sensor_green_s3, sensor_blue_s3;
unsigned int sensor_red_s4, sensor_green_s4, sensor_blue_s4;

// declare temp sensor values for all 4 sectors, source: arduino
unsigned int sensor_temp_s1, sensor_temp_s2, sensor_temp_s3, sensor_temp_s4;

// declare status/alarms that we will flag during startup and opertaion, source: Arduino
unsigned int Driver_status_s1,Driver_status_s2,Driver_status_s3,Driver_status_s4; 
unsigned int RGBsensor_status_s1,RGBsensor_status_s2,RGBsensor_status_s3,RGBsensor_status_s4;
unsigned int temp_status_s1, temp_status_s2, temp_status_s3, temp_status_s4;



 

void setup() {
  Serial.begin(115200);
	Wire.begin(8);                // join i2c bus with address #8
  Serial.println("Wire.begin Initiated");
	Wire.onRequest(requestEvent); // register event
  Serial.println("Wire.onRequest set");
}

void loop() {
	intensity_red_s1 = 0xA4C1; intensity_green_s1= 0xA4C1; intensity_blue_s1 = 0xA4C1;
	intensity_red_s2, intensity_green_s2, intensity_blue_s2 = 0xFF2;
	intensity_red_s3, intensity_green_s3, intensity_blue_s3 = 0xFF3;
	intensity_red_s4, intensity_green_s4, intensity_blue_s4 = 0xFF4;
	sensor_red_s1, sensor_green_s1, sensor_blue_s1 = 0xFA1;
	sensor_red_s2, sensor_green_s2, sensor_blue_s2 = 0xFA2;
	sensor_red_s3, sensor_green_s3, sensor_blue_s3 = 0xFA3;
	sensor_red_s4, sensor_green_s4, sensor_blue_s4 = 0xFA4;
	sensor_temp_s1, sensor_temp_s2, sensor_temp_s3, sensor_temp_s4 = 0x1234;
	Driver_status_s1,Driver_status_s2,Driver_status_s3,Driver_status_s4 = 0x1;
	RGBsensor_status_s1,RGBsensor_status_s2,RGBsensor_status_s3,RGBsensor_status_s4 = 0x0;
	temp_status_s1, temp_status_s2, temp_status_s3, temp_status_s4 = 0x1;
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  delay(100);
  Serial.println("requestEvent() was initiated");
	unsigned int sensorArray[] = {intensity_red_s1, intensity_green_s1, intensity_blue_s1};//,
//		intensity_red_s2, intensity_green_s2, intensity_blue_s2,
//		intensity_red_s3, intensity_green_s3, intensity_blue_s3,
//		intensity_red_s4, intensity_green_s4, intensity_blue_s4,
//		sensor_red_s1, sensor_green_s1, sensor_blue_s1,
//		sensor_red_s2, sensor_green_s2, sensor_blue_s2,
//		sensor_red_s3, sensor_green_s3, sensor_blue_s3,
//		sensor_red_s4, sensor_green_s4, sensor_blue_s4,
//		sensor_temp_s1, sensor_temp_s2, sensor_temp_s3, sensor_temp_s4,
//		Driver_status_s1,Driver_status_s2,Driver_status_s3,Driver_status_s4,
//		RGBsensor_status_s1,RGBsensor_status_s2,RGBsensor_status_s3,RGBsensor_status_s4,
//		temp_status_s1, temp_status_s2, temp_status_s3, temp_status_s4};
  int index = 6;
	byte sendArray[index] = {};
  for (int i=0; i<3;i++){
		sendArray[i*2]= lowByte(sensorArray[i]);
		sendArray[(i*2)+1]= highByte(sensorArray[i]);
    Serial.println(sendArray[i]);
  }
   Serial.println("sendArray size: ");
   Serial.println(sizeof(sendArray));
	
	Wire.write(sendArray,6); 
}
