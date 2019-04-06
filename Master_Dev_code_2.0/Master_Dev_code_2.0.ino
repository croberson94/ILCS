#include <SoftwareWire.h>
#include <ISL29125_SoftWire.h>
#include <Wire.h>
#include <RGBLEDLighting.h>

#define ADDRESS                    0x15E

InfineonRGB LEDS;  // Create Object



/*
 *Declare All Global Variables
 */



// Light sensor variables
unsigned int red_1;
unsigned int green_1;
unsigned int blue_1;
unsigned int red_2;
unsigned int green_2;
unsigned int blue_2;
unsigned int red_3;
unsigned int green_3;
unsigned int blue_3;
unsigned int red_4;
unsigned int green_4;
unsigned int blue_4;

// RGB Values for light output cycle
const unsigned int day_int_val[3] = {0x555,0x555,0x555};
const unsigned int morning_int_val[3] = {0x555,0x555,0x555};
const unsigned int evening_int_val[3] = {0x555,0x555,0x555};
const unsigned int night_int_val[3] = {0x0,0x0,0x0};

//increment value
unsigned int i = 0;
byte received;
byte code;
// Cycle Status Variables
int cyc_status1 = 0xFF;


// Arrays for iteration
const unsigned int dimming_array[24] = {0x0,0x0,0x0,0x0,0x19A,0x4CC,0x665,0x998,0xCCB,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,
0xCCB,0x998,0x665,0x4CC,0x19A,0x0,0x0,0x0,0x0};
unsigned int red_int_val[24] = {night_int_val[0],night_int_val[0],night_int_val[0],night_int_val[0],morning_int_val[0],
morning_int_val[0],day_int_val[0],day_int_val[0],day_int_val[0],day_int_val[0],day_int_val[0],
day_int_val[0],day_int_val[0],day_int_val[0],day_int_val[0],day_int_val[0],day_int_val[0],day_int_val[0],evening_int_val[0],
evening_int_val[0],night_int_val[0],night_int_val[0],night_int_val[0],night_int_val[0]};

unsigned int green_int_val[24] = {night_int_val[1],night_int_val[1],night_int_val[1],night_int_val[1],morning_int_val[1],
morning_int_val[1],day_int_val[1],day_int_val[1],day_int_val[1],day_int_val[1],day_int_val[1],
day_int_val[1],day_int_val[1],day_int_val[1],day_int_val[1],day_int_val[1],day_int_val[1],day_int_val[1],evening_int_val[1],
evening_int_val[1],night_int_val[1],night_int_val[1],night_int_val[1],night_int_val[1]};

unsigned int blue_int_val[24] = {night_int_val[2],night_int_val[2],night_int_val[2],night_int_val[2],morning_int_val[2],
morning_int_val[2],day_int_val[2],day_int_val[2],day_int_val[2],day_int_val[2],day_int_val[2],
day_int_val[2],day_int_val[2],day_int_val[2],day_int_val[2],day_int_val[2],day_int_val[2],day_int_val[2],evening_int_val[2],
evening_int_val[2],night_int_val[2],night_int_val[2],night_int_val[2],night_int_val[2]};

const int current[24]= {0x0,0x0,0x0,0x0,0x2D,0x2D,0x2D,0x2D,0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x2D,0x2D,0x2D,0x2D,
  0x0,0x0,0x0,0x0};

const int offtime[24] = {0xFF,0xFF,0xFF,0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0xFF,0xFF,0xFF};

//Analog Inputs for Temperature sensors
int LMT86_0 = A0; 
int LMT86_1 = A1;
int LMT86_2 = A2;
int LMT86_3 = A3;
int PinA0 = 0;
int PinA1 = 0;
int PinA2 = 0;
int PinA3 = 0;
float TmpC_0 = 0;
float TmpC_1 = 0;
float TmpC_2 = 0;
float TmpC_3 = 0;
float TmpF_0 = 0;
float TmpF_1 = 0;
float TmpF_2 = 0;
float TmpF_3 = 0;
float voltage_0 = 0;
float voltage_1 = 0;
float voltage_2 = 0;
float voltage_3 = 0;
float mV_0 = 0;
float mV_1 = 0;
float mV_2 = 0;
float mV_3 = 0;
// mV differrence per degree in Celsius
const float mV_PerC = 0.01105;


// Declare sensor object
ISL29125_SOFT RGB_sensor[3];


/*
 * Declare the Light Driver Objects with desired (SDA,SCL) pins
*/

SoftwareWire myWire1 ( 2, 3);
SoftwareWire myWire2 ( 4, 5);
SoftwareWire myWire3 ( 6, 7);
SoftwareWire myWire4 ( 8, 9);





void GUIhandler(int howMany){
  
  byte code = Wire.read();
  /*
  switch (lightcode){
    case 0:
      isrSector1(code);
      break;
}
*/

  
}
void isrSector1(int code) {
  // change or initialize sector1 light fixture
  if(code == cyc_status1){
    i++;
    if(i == 24){i=0;}
  }
  else{
    //decode instructions in variable code
    cyc_status1 = code;
    if(i == 24){i=0;}
  }
}



void GUIoutput() {
  delay(100);
	unsigned int sensorArray[] = {red_1,green_1,blue_1,red_2,green_2,blue_2,red_3,green_3,blue_3,red_4,green_4,blue_4};
  int index = 24;
	byte sendArray[index] = {};
  for (int i=0; i<12;i++){
		sendArray[i*2]= lowByte(sensorArray[i]);
		sendArray[(i*2)+1]= highByte(sensorArray[i]);
  }
	Wire.write(sendArray,index);
}


void setup() {
  //Initialize all I2C buses for light drivers
  myWire1.begin(); // join i2c bus (address optional for master)
  myWire2.begin();
  myWire3.begin();
  myWire4.begin();
  Serial.begin(115200);
  
  //Initialize I2C bus for 4Duino Communication
  Wire.begin(8);
  Wire.onReceive(GUIhandler);
  Wire.onRequest(GUIoutput);
  
  //Save Parameter calls, test one at a time
  /*
  I2CSAVEPARAM_1(ADDRESS);
 
  Serial.begin(115200);
  Serial.println(i);
  Serial.println(red_int_val[i]);
  Serial.println(green_int_val[i]);
  Serial.println(blue_int_val[i]);
  
   /*
   *Initialize each sensor and return the results of the initialization
   */
  /*
  if (RGB_sensor[0].init(22,23)) 
  {
    Serial.println("Sensor_1 Initialization Successful\n\r");
  }
  else{
    Serial.println("Sensor_1 Initialization Failed\n");
  }
  
  if (RGB_sensor[1].init(24,25)) 
  {
    Serial.println("Sensor_2 Initialization Successful\n\r");
  }
  else{
    Serial.println("Sensor_2 Initialization Failed\n");
  }
  
    if (RGB_sensor[2].init(26,27)) 
  {
    Serial.println("Sensor_3 Initialization Successful\n\r");
  }
  else{
    Serial.println("Sensor_3 Initialization Failed\n");
  }
  
    if (RGB_sensor[3].init(28,29)) 
  {
    Serial.println("Sensor_4 Initialization Successful\n\r");
  }
  else{
    Serial.println("Sensor_4 Initialization Failed\n");
  }
*/
  // GLOBAL LED DRIVER COMMANDS
  int glob_fadrate = 0xEA6;
		int glob_walktime = 0x186;
		int read_faderate = 0;
		int read_walktime = 0;
  //set faderate
  LEDS.I2CWRITE2BYTES(ADDRESS,FADERATE,glob_fadrate); 
  //set walktime
  LEDS.I2CWRITE2BYTES(ADDRESS,WALKTIME,glob_walktime);
		
		read_walktime = LEDS.I2CREAD(ADDRESS, READ_WALKTIME);
		read_faderate = LEDS.I2CREAD(ADDRESS, READ_FADERATE);
		
		
		
		

  
 
}

void loop() {

  /*
   * Variable State LED Driver commands, state is controlled by isrSector* commands
  */
  
  
  received = code;
  Serial.println(received);
  Serial.println(i);
  Serial.println(red_int_val[i]);
  Serial.println(green_int_val[i]);
  Serial.println(blue_int_val[i]);
  Serial.println(dimming_array[i]);
  Serial.println(current[i]);
  Serial.println(offtime[i]);
  
  // SECTOR 1
  
  LEDS.SetIntensityRGB(red_int_val[i],red_int_val[i],red_int_val[i]);
  LEDS.I2CWRITE2BYTES(ADDRESS,DIMMINGLEVEL,dimming_array[i]);
  LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_RED, current[i]);
  LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_BLUE, current[i]);
  LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_GREEN, current[i]);
  LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_RED, offtime[i]);
  LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_BLUE, offtime[i]);
  LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_GREEN, offtime[i]);
  

  //TEST SCRIPT ONLY
  i++;
 

  if(i == 24){i=0;}
    
  delay(2500);
  
  //Serial.println(i);
  //Serial.println(i_2);
 
  /*
   * CONSTANT SENSOR READINGS
   */
  /*
  //Light Sensor

  // Read Sector_1 values (16 bit integers)
  red_1 = RGB_sensor[0].readRed();
  green_1 = RGB_sensor[0].readGreen();
  blue_1 = RGB_sensor[0].readBlue();
  
  // Read Sector_2 values (16 bit integers)
  red_2 = RGB_sensor[1].readRed();
  green_2 = RGB_sensor[1].readGreen();
  blue_2 = RGB_sensor[1].readBlue();
  
  // Read Sector_3 values (16 bit integers)
  red_3 = RGB_sensor[2].readRed();
  green_3 = RGB_sensor[2].readGreen();
  blue_3 = RGB_sensor[2].readBlue();
  
  // Read Sector_4 values (16 bit integers)
  red_4 = RGB_sensor[3].readRed();
  green_4 = RGB_sensor[3].readGreen();
  blue_4 = RGB_sensor[3].readBlue();
  /*
  //Temperature Sensor
  PinA0 = analogRead(LMT86_0);         // read value from pin A0  
  voltage_0 = PinA0 * (4.9 / 1023.0);
  mV_0 = 2.1 - voltage_0;              // starting point for temperature 0 celsius 
  TmpC_0 = mV_0 / mV_PerC;             // temperature conversion for celsius 
  TmpF_0 = TmpC_0 * 1.8 + 32;          // temperature conversion for farenheit 

  Serial.print("Sensor 1: ");          // print Sensor 1 
  Serial.print(voltage_0, 4);         // print voltage value for Sensor 1 
  Serial.print("Vdc");              
  Serial.print(", ");
  Serial.print(TmpC_0);               // print temperature in Celsius 
  Serial.print(" C"); 
  Serial.print(", ");
  Serial.print(TmpF_0);              // print temperature in farenheit 
  Serial.println(" F");
                         
  
  PinA1 = analogRead(LMT86_1);       // read value from pin A1
  voltage_1 = PinA1 * (4.9 / 1023.0);
  mV_1 = 2.1 - voltage_1;            // starting point for temperature 0 celsius 
  TmpC_1 = mV_1 / mV_PerC;           // temperature conversion for celsius
  TmpF_1 = TmpC_1 * 1.8 + 32;       // temperature conversion for farenheit 

  Serial.print("Sensor 2: ");      // print Sensor 2
  Serial.print(voltage_1, 4);      // print voltage for Sensor 2
  Serial.print("Vdc");
  Serial.print(", ");
  Serial.print(TmpC_1);           // print temperature in Celsius 
  Serial.print(" C");
  Serial.print(", ");
  Serial.print(TmpF_1);           // print temperature in Farenheit 
  Serial.println(" F");
                   
  
  PinA2 = analogRead(LMT86_2);       // read value from pin A2
  voltage_2 = PinA2 * (4.9 / 1023.0); 
  mV_2 = 2.1 - voltage_2;            // starting point for temperature 0 celsius
  TmpC_2 = mV_2 / mV_PerC;           // temperature conversion for celsius
  TmpF_2 = TmpC_2 * 1.8 + 32;        // temperature conversion for farenheit

  Serial.print("Sensor 3: ");       // print Sensor 3
  Serial.print(voltage_2, 4);       // print voltage for Sensor 3
  Serial.print("Vdc");
  Serial.print(", ");
  Serial.print(TmpC_2);            // print Temperature in Celsius
  Serial.print(" C");
  Serial.print(", ");
  Serial.print(TmpF_2);           // print temperature in Farenheit
  Serial.println(" F");
 
  
  PinA3 = analogRead(LMT86_3);        // read value from pin A3
  voltage_3 = PinA3 * (4.9 / 1023.0);
  mV_3 = 2.1 - voltage_3;            // starting point for temperature 0 celsius
  TmpC_3 = mV_3 / mV_PerC;           // temperature conversion for celsius
  TmpF_3 = TmpC_3 * 1.8 + 32;        // temperature conversion for farenheit

  Serial.print("Sensor 4: ");        // print sensor 4 
  Serial.print(voltage_3, 4);        // pint voltage for Sensor 4
  Serial.print("Vdc");
  Serial.print(", ");
  Serial.print(TmpC_3);              // print Temperature in Celsius 
  Serial.print(" C");
  Serial.print(", ");
  Serial.print(TmpF_3);              // print Temperature in Farenheit
  Serial.println(" F");
  delay(1000);
  
  
  /* 
  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("Sensor 1 Red: "); Serial.println(red_1,HEX);
  Serial.print("Sensor 1 Green: "); Serial.println(green_1,HEX);
  Serial.print("Sensor 1 Blue: "); Serial.println(blue_1,HEX);
  Serial.println();
  delay(1000);
    // Read sensor_1 values (16 bit integers)
  red_1 = RGB_sensor[0].readRed();
  green_1 = RGB_sensor[0].readGreen();
  blue_1 = RGB_sensor[0].readBlue();

  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("Sensor 1 Red: "); Serial.println(red_1,HEX);
  Serial.print("Sensor 1 Green: "); Serial.println(green_1,HEX);
  Serial.print("Sensor 1 Blue: "); Serial.println(blue_1,HEX);
  Serial.println();
  delay(1000);
  

  // Read sensor_1 values (16 bit integers)
  red_1 = RGB_sensor[0].readRed();
  green_1 = RGB_sensor[0].readGreen();
  blue_1 = RGB_sensor[0].readBlue();
  delay(2000);
  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("Sensor 1 Red: "); Serial.println(red_1,HEX);
  Serial.print("Sensor 1 Green: "); Serial.println(green_1,HEX);
  Serial.print("Sensor 1 Blue: "); Serial.println(blue_1,HEX);
  Serial.println();

  Serial.println("The light is blue");
  // Read sensor_1 values (16 bit integers)
  red_1 = RGB_sensor[0].readRed();
  green_1 = RGB_sensor[0].readGreen();
  blue_1 = RGB_sensor[0].readBlue();

  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("Sensor 1 Red: "); Serial.println(red_1,HEX);
  Serial.print("Sensor 1 Green: "); Serial.println(green_1,HEX);
  Serial.print("Sensor 1 Blue: "); Serial.println(blue_1,HEX);
  Serial.println();
  
  
  Serial.print("Sensor 2 Red: "); Serial.println(red_2,HEX);
  Serial.print("Sensor 2 Green: "); Serial.println(green_2,HEX);
  Serial.print("Sensor 2 Blue: "); Serial.println(blue_2,HEX);
  Serial.println();
  */
  
  
}
