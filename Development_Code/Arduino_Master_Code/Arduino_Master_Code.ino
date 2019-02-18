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
 *					
 * 
 * Last Revision: 02/13/2019
 * 
 */

#include <Wire.h>
//int index = 80;
byte receiveArray[80] = {};
unsigned int sensorArray[40]={};
//String labelArray[100]={"Red-Sector-1: ","Green-Sector-1: ","Blue-Sector-1: ",
//	"Red-Sector-2: ","Green-Sector-2: ","Blue-Sector-2: ",
//	"Red-Sector-3: ","Green-Sector-3: ","Blue-Sector-3: ",
//	"Red-Sector-4: ","Green-Sector-4: ","Blue-Sector-4: ",
//	"Sensor-Red-1: ","Sensor-Green-1: ","Sensor-Blue-1: ",
//	"Sensor-Red-2: ","Sensor-Green-2: ","Sensor-Blue-2: ",
//	"Sensor-Red-3: ","Sensor-Green-3: ","Sensor-Blue-3: ",
//	"Sensor-Red-4: ","Sensor-Green-4: ","Sensor-Blue-4: ",
//	"Temp-Sensor-1: ","Temp-Sensor-2: ","Temp-Sensor-3: ","Temp-Sensor-4: ",
//	"Driver_status_s1: ","Driver_status_s2: ","Driver_status_s3: ","Driver_status_s4: ",
//	"RGBSensor_status_s1: ","RGBSensor_status_s2: ","RGBSensor_status_s3: ","RGBSensor_status_s4: ",
//	"Temp-Sensor-1: ","Temp-Sensor-2: ","Temp-Sensor-3: ","Temp-Sensor-4: "
//};

void setup() {
	Wire.begin();        			// join i2c bus (address optional for master)
	Serial.begin(115200);  			// start serial for output
  
}

void loop() {
	int i =0;
	Wire.requestFrom(8, 8);    	// request 6 bytes from slave device #8
	Serial.println("request was made");
//  for (int i =0;i<80;i++){
//    receiveArray[i] = Wire.read();
//    Serial.println(receiveArray[i]);
//    Serial.print("wire received ");
//    Serial.println(i);
//  }
  
	while (Wire.available()) { 		// slave may send less than requested
		receiveArray[i] = Wire.read(); 		// receive a byte as character
    Serial.println(String(receiveArray[i],HEX));
    Serial.print("wire recieved ");
    Serial.println(i);
    i++;                  // print the character
  }
	for (int j=0; j<6; j++){
		sensorArray[j] = (receiveArray[(j*2)+1]*256) + receiveArray[j*2];
	}
	for (int k=0; k<3; k++){
		Serial.print(k);
    Serial.print("->");
		Serial.println(String(sensorArray[k],HEX));
	}
	delay(6000);
}
 
 //unsigned int word = high_byte * 256 + low_byte;
