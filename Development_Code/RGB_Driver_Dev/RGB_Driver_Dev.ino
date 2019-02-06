
/*
 * The University of Texas at Tyler
 * Intellegent Lighting Control System Team
 * 
 * Purpose:       This script builds circadian cycles and sends commands
 *                to the infineon RGB LED Lighting shield
 *                
 * Author:        Griggs Hutaff
 * 
 * Last Revision: 02/06/2019
 * 
 */





#include <RGBLEDLighting.h>
#include <Wire.h>

InfineonRGB LEDS;  // Create Object

  //declare all variables needed

//  int intensity_red, intensity_green, intensity_blue;
//  int current_red, current_green, current_blue;
//  int sensor_red, int sensor_green, int sensor_blue;
//  int tempsens1, int tempsens2, int tempsens3, int tempsens4;
//  int sctr1_addr, sctr2_addr, sctr3_addr, sctr4_addr;
    int sctr1_addr = 0x15E;
    int offtime_read;
/*
 * time_multiplier allows us to quickly adjust the cycle run time,
 * the multipliers will be in orders of 10:
 * 1 = 6sec cycle
 * 10 = 60 sec cycle
 * 100 = 10 minutes
 * 600 = 1 hour
 * 14,400 = 24 hours
 */
    float time_multiplier;

/*
 * dimming values will be placed in an array, this array may be prestored during  
 * development OR this array may be generated by an algorithm based on initial conditions
 * and time constraints
 */
    int dimming_array[] = {0x19A,0x332,0x4CC,0x665,0x7FF,0x998,0xB31,0xCCB,0xE64,0xFFF};
    char* percent_array[] = {"10%","20%","30%","40%","50%","60%","70%","80%","90%","100%"};
/*
 * Color intensities will also need to be put in any array, this is more challenging because
 * we will have a 2 dimentional list
 */
    int day_intense[3] = {0x555,0x555,0x640};
    int morning_intense[3] = {0x555,0x555,0x3E8};

    int red_intense[10] = {morning_intense[0],morning_intense[0],morning_intense[0],morning_intense[0],morning_intense[0],
    day_intense[0],day_intense[0],day_intense[0],day_intense[0],day_intense[0]};
    
    int green_intense[10] = {morning_intense[1],morning_intense[1],morning_intense[1],morning_intense[1],morning_intense[1],
    day_intense[1],day_intense[1],day_intense[1],day_intense[1],day_intense[1]};
    
    int blue_intense[10] = {morning_intense[2],morning_intense[2],morning_intense[2],morning_intense[2],morning_intense[2],
    day_intense[2],day_intense[2],day_intense[2],day_intense[2],day_intense[2]};

    //this was a failed attempt to creat a lol for the light intensity values
//  int (*day_ptr)[3];
//   for (int i =0;i<3;i++){day_ptr[i]= &day_intense[i]}
//    int (*morn_ptr)[3];
////    for (int i =0;i<3;i++){
////      morn_ptr[i]= &morning_intense[i];
////      }
//   
////    int intensities[] = {morn_ptr,morn_ptr,morn_ptr,morn_ptr,morn_ptr,day_ptr,day_ptr,day_ptr,day_ptr,day_ptr};
      
 
void setup() {

   Serial.begin(38400); // Starts the serial connection at 38400 baud

/*
 * For now it seems that Wire.begin() is the only usable function to come out of the LEDS.begin() class,
 * as we proceed we may have to look again to see if we need to anything else to the driver initialization
 */
       
  Wire.begin();
  
/*
 * WALKTIME:
 *The RGB LED Shield calculates the actual linear walktime with the formula:
 *Linear Walk Time = WALKTIME * 0.0124
  */
  
  LEDS.I2CWRITE2BYTES(sctr1_addr,WALKTIME,0x186);  // Set walk time to 2 seconds
/*
 * DIMMING LEVEL:
 * The curve is quantized into 4095 steps, pseudo exponential curve.
 * *NOTE* The brightness value of a channel = intesity*diming level/4096
 */
   //LEDS.I2CWRITE2BYTES(sctr1_addr,DIMMINGLEVEL,0x19A); // 10% Brightness
   //LEDS.I2CWRITE2BYTES(sctr1_addr,DIMMINGLEVEL,0x555); //50% brightness
   //LEDS.SetDimmingLevel(0x0555);

/* 
 *  Current Level maximum is 0x80
 */
   LEDS.I2CWRITE2BYTES(sctr1_addr,CURRENT_RED, 0x2D);
   LEDS.I2CWRITE2BYTES(sctr1_addr,CURRENT_BLUE, 0x2D);
   LEDS.I2CWRITE2BYTES(sctr1_addr,CURRENT_GREEN, 0x2D);
   LEDS.I2CWRITE2BYTES(sctr1_addr,FADERATE,0xEA6);  // Set faderate

   //set offtime
   LEDS.I2CWRITE2BYTES(sctr1_addr,OFFTIME_RED, 0x28);
   LEDS.I2CWRITE2BYTES(sctr1_addr,OFFTIME_GREEN, 0x28);
   LEDS.I2CWRITE2BYTES(sctr1_addr,OFFTIME_BLUE, 0x28);
}

void loop() {
  Serial.println("The Loop is reset");
  //LEDS.SetIntensityRGB(0x0555, 0x0555, 0x0555);
  time_multiplier = 10;
  for (int i = 0; i<10;i++)
  {
    LEDS.SetIntensityRGB(red_intense[i], green_intense[i], blue_intense[i]);
    LEDS.I2CWRITE2BYTES(sctr1_addr,DIMMINGLEVEL,dimming_array[i]);
    Serial.print("The Brightness is ");
    Serial.println(percent_array[i]);
    Serial.println(dimming_array[i]);
    offtime_read = LEDS.I2CREAD(sctr1_addr,READ_OFFTIME_RED);
    Serial.print("The offtime for read channel is ");
    Serial.println(offtime_read);
//    Serial.println(local_day[1]);
//    Serial.println(local_day[2]);
//    Serial.println(local_day[3]);
    delay(300*time_multiplier);
  }
  for (int i= 9;i>1;i--)
  { 
    LEDS.SetIntensityRGB(red_intense[i], green_intense[i], blue_intense[i]);
    LEDS.I2CWRITE2BYTES(sctr1_addr,DIMMINGLEVEL,dimming_array[i-1]);
    Serial.print("The Brightness is ");
    Serial.println(percent_array[i-1]);
    Serial.println(dimming_array[i-1]);
    delay(300*time_multiplier);
  }
  

}