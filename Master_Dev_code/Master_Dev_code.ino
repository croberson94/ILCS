#include <SoftwareWire.h>
#include <ISL29125_SoftWire.h>
#include <Wire.h>

#define ADDRESS                    0x15EUL

#define INTENSITY_RED              0x11U
#define INTENSITY_GREEN            0x12U
#define INTENSITY_BLUE             0x13U
#define CURRENT_RED                0x21U
#define CURRENT_GREEN              0x22U
#define CURRENT_BLUE               0x23U
#define OFFTIME_RED                0x41U
#define OFFTIME_GREEN              0x42U
#define OFFTIME_BLUE               0x43U
#define WALKTIME                   0x50U
#define DIMMINGLEVEL               0x60U
#define FADERATE                   0x61U
#define SAVEPARAMETERS             0xA0U


//private variables from header file
unsigned int message;
unsigned int c[2] = {0};
/*
 *Declare All Global Variables
 */

// Time Multiplier will change cycle duration
float time_multiplier1;
float time_multiplier2;
float time_multiplier3;
float time_multiplier4;

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

//increment values
unsigned int i_1 = 0;
unsigned int i_2 = 5;
unsigned int i_3 = 10;
unsigned int i_4 = 15;

// Cycle Status Variables
int cyc_status1 = 0xFF;
int cyc_status2 = 0xFF;
int cyc_status3 = 0xFF;
int cyc_status4 = 0xFF;

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

const int offtime[24] = {0xFF,0xFF,0xFF,0xFF,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xFF,0xFF,0xFF,0xFF};

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

/*
 *CONJUNCTION FUNCTIONS
 */

void I2CSAVEPARAM_1 (unsigned int Address)
{
  int i = 0;
  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF);
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79;

  myWire1.beginTransmission(byte(upperSLAD));
  myWire1.write(byte(lowerSLAD));
  myWire1.write(byte(SAVEPARAMETERS)); // write SAVEPARAMETERS command
  myWire1.endTransmission(false); // false for Repeated Start

  myWire1.beginTransmission(byte(upperSLAD));
  myWire1.write(byte(lowerSLAD)); // write to address lower 8 bits of slave address
  myWire1.requestFrom(upperSLAD, 2, true);  // send READ request with upper slave address
  unsigned int data = 0;

  while (myWire1.available())   // slave may send less than requested. Print out received data byte
  {
    c[i] = myWire1.read(); // receive a byte as character
    i++;
  }
  myWire1.endTransmission(true); // STOP condition
  data = c[1]; // print the data on serial monitor
  data = (data << 8) | c[0];
  //Serial.print("0x");
  //if (data < 0x1000)
    //Serial.print("0");
    //Serial.println(data, HEX);

}

void I2CSAVEPARAM_2 (unsigned int Address)
{
  int i = 0;
  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF);
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79;

  myWire2.beginTransmission(byte(upperSLAD));
  myWire2.write(byte(lowerSLAD));
  myWire2.write(byte(SAVEPARAMETERS)); // write SAVEPARAMETERS command
  myWire2.endTransmission(false); // false for Repeated Start

  myWire2.beginTransmission(byte(upperSLAD));
  myWire2.write(byte(lowerSLAD)); // write to address lower 8 bits of slave address
  myWire2.requestFrom(upperSLAD, 2, true);  // send READ request with upper slave address
  unsigned int data = 0;

  while (myWire2.available())   // slave may send less than requested. Print out received data byte
  {
    c[i] = myWire2.read(); // receive a byte as character
    i++;
  }
  myWire2.endTransmission(true); // STOP condition
  data = c[1]; // print the data on serial monitor
  data = (data << 8) | c[0];
  //Serial.print("0x");
  //if (data < 0x1000)
    //Serial.print("0");
    //Serial.println(data, HEX);

}

void I2CSAVEPARAM_3 (unsigned int Address)
{
  int i = 0;
  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF);
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79;

  myWire3.beginTransmission(byte(upperSLAD));
  myWire3.write(byte(lowerSLAD));
  myWire3.write(byte(SAVEPARAMETERS)); // write SAVEPARAMETERS command
  myWire3.endTransmission(false); // false for Repeated Start

  myWire3.beginTransmission(byte(upperSLAD));
  myWire3.write(byte(lowerSLAD)); // write to address lower 8 bits of slave address
  myWire3.requestFrom(upperSLAD, 2, true);  // send READ request with upper slave address
  unsigned int data = 0;

  while (myWire3.available())   // slave may send less than requested. Print out received data byte
  {
    c[i] = myWire3.read(); // receive a byte as character
    i++;
  }
  myWire3.endTransmission(true); // STOP condition
  data = c[1]; // print the data on serial monitor
  data = (data << 8) | c[0];
  //Serial.print("0x");
  //if (data < 0x1000)
    //Serial.print("0");
    //Serial.println(data, HEX);

}

void I2CSAVEPARAM_4 (unsigned int Address)
{
  int i = 0;
  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF);
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79;

  myWire4.beginTransmission(byte(upperSLAD));
  myWire4.write(byte(lowerSLAD));
  myWire4.write(byte(SAVEPARAMETERS)); // write SAVEPARAMETERS command
  myWire4.endTransmission(false); // false for Repeated Start

  myWire4.beginTransmission(byte(upperSLAD));
  myWire4.write(byte(lowerSLAD)); // write to address lower 8 bits of slave address
  myWire4.requestFrom(upperSLAD, 2, true);  // send READ request with upper slave address
  unsigned int data = 0;

  while (myWire4.available())   // slave may send less than requested. Print out received data byte
  {
    c[i] = myWire4.read(); // receive a byte as character
    i++;
  }
  myWire4.endTransmission(true); // STOP condition
  data = c[1]; // print the data on serial monitor
  data = (data << 8) | c[0];
  //Serial.print("0x");
  //if (data < 0x1000)
    //Serial.print("0");
    //Serial.println(data, HEX);

}

void I2CWRITE2BYTES_1(int Address, int Command, unsigned int Data)
{
  unsigned int upperByte, lowerByte; // Separate 4 byte data into 2 byte values
  lowerByte = Data;
  upperByte = Data >> 8;

  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF); // Putting address into correct format
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write

  myWire1.beginTransmission(byte(upperSLAD)); // Start I2C transmission
  myWire1.write(byte(lowerSLAD)); // address lower 8 bits of i2c address
  myWire1.write(byte(Command)); // write command
  myWire1.write(byte(upperByte)); // write data
  myWire1.write(byte(lowerByte));
  myWire1.endTransmission(true);

}

void SetIntensityRGB_1(unsigned int Red, unsigned int Green, unsigned int Blue){
  I2CWRITE2BYTES_1(ADDRESS,INTENSITY_RED, Red);
  I2CWRITE2BYTES_1(ADDRESS,INTENSITY_GREEN, Green);
  I2CWRITE2BYTES_1(ADDRESS,INTENSITY_BLUE, Blue);
}

void I2CWRITE2BYTES_2(int Address, int Command, unsigned int Data)
{
  unsigned int upperByte, lowerByte; // Separate 4 byte data into 2 byte values
  lowerByte = Data;
  upperByte = Data >> 8;

  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF); // Putting address into correct format
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write

  myWire2.beginTransmission(byte(upperSLAD)); // Start I2C transmission
  myWire2.write(byte(lowerSLAD)); // address lower 8 bits of i2c address
  myWire2.write(byte(Command)); // write command
  myWire2.write(byte(upperByte)); // write data
  myWire2.write(byte(lowerByte));
  myWire2.endTransmission(true);

}

void SetIntensityRGB_2(unsigned int Red, unsigned int Green, unsigned int Blue){
  I2CWRITE2BYTES_2(ADDRESS,INTENSITY_RED, Red);
  I2CWRITE2BYTES_2(ADDRESS,INTENSITY_GREEN, Green);
  I2CWRITE2BYTES_2(ADDRESS,INTENSITY_BLUE, Blue);
}

void I2CWRITE2BYTES_3(int Address, int Command, unsigned int Data)
{
  unsigned int upperByte, lowerByte; // Separate 4 byte data into 2 byte values
  lowerByte = Data;
  upperByte = Data >> 8;

  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF); // Putting address into correct format
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write

  myWire3.beginTransmission(byte(upperSLAD)); // Start I2C transmission
  myWire3.write(byte(lowerSLAD)); // address lower 8 bits of i2c address
  myWire3.write(byte(Command)); // write command
  myWire3.write(byte(upperByte)); // write data
  myWire3.write(byte(lowerByte));
  myWire3.endTransmission(true);

}

void SetIntensityRGB_3(unsigned int Red, unsigned int Green, unsigned int Blue){
  I2CWRITE2BYTES_3(ADDRESS,INTENSITY_RED, Red);
  I2CWRITE2BYTES_3(ADDRESS,INTENSITY_GREEN, Green);
  I2CWRITE2BYTES_3(ADDRESS,INTENSITY_BLUE, Blue);
}

void I2CWRITE2BYTES_4(int Address, int Command, unsigned int Data)
{
  unsigned int upperByte, lowerByte; // Separate 4 byte data into 2 byte values
  lowerByte = Data;
  upperByte = Data >> 8;

  unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF); // Putting address into correct format
  unsigned int upperSLAD = Address >> 8;
  upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write

  myWire4.beginTransmission(byte(upperSLAD)); // Start I2C transmission
  myWire4.write(byte(lowerSLAD)); // address lower 8 bits of i2c address
  myWire4.write(byte(Command)); // write command
  myWire4.write(byte(upperByte)); // write data
  myWire4.write(byte(lowerByte));
  myWire4.endTransmission(true);

}

void SetIntensityRGB_4(unsigned int Red, unsigned int Green, unsigned int Blue){
  I2CWRITE2BYTES_4(ADDRESS,INTENSITY_RED, Red);
  I2CWRITE2BYTES_4(ADDRESS,INTENSITY_GREEN, Green);
  I2CWRITE2BYTES_4(ADDRESS,INTENSITY_BLUE, Blue);
}



void GUIhandler(int howMany){
  byte code = Wire.read();
  int lightcode = code & 0x3;
  switch (lightcode){
    case 0:
      isrSector1(code);
      break;
    case 1:
      isrSector2(code);
      break;
    case 2:
      isrSector3(code);
      break;
    case 3:
      isrSector4(code);
      break;
  }
  
}
void isrSector1(int code) {
  // change or initialize sector1 light fixture
  if(code == cyc_status1){
    i_1++;
    if(i_1 == 24){i_1=0;}
  }
  else{
    //decode instructions in variable code
    cyc_status1 = code;
    if(i_1 == 24){i_1=0;}
  }
}

void isrSector2(int code) {
  // change or initialize sector2 light fixture
  if(code == cyc_status2){
    i_2++;
    if(i_2 == 24){i_2=0;}
  }
  else{
    //decode instructions in variable code
    cyc_status2 = code;
    if(i_1 == 24){i_1=0;}
  }
}

void isrSector3(int code) {
  // change or initialize sector3 light fixture
  if(code == cyc_status3){
    i_3++;
    if(i_3 == 24){i_3=0;}
  }
  else{
    //decode instructions in variable code
    cyc_status3 = code;
    if(i_1 == 24){i_1=0;}
  }
}

void isrSector4(int code) {
  // change or initialize sector4 light fixture
  if(code == cyc_status4){
    i_4++;
    if(i_4 == 24){i_4=0;}
  }
  else{
    //decode instructions in variable code
    cyc_status4 = code;
    if(i_1 == 24){i_1=0;}
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
  
  //Initialize I2C bus for 4Duino Communication
  Wire.begin(8);
  Wire.onReceive(GUIhandler);
  Wire.onRequest(GUIoutput);
  
  //Save Parameter calls, test one at a time
  /*
  I2CSAVEPARAM_1(ADDRESS);
  I2CSAVEPARAM_2(ADDRESS);
  I2CSAVEPARAM_3(ADDRESS);
  I2CSAVEPARAM_4(ADDRESS);
  */
  Serial.begin(115200);
  Serial.println(i_1);
  Serial.println(red_int_val[i_1]);
  Serial.println(green_int_val[i_1]);
  Serial.println(blue_int_val[i_1]);
  
   /*
   *Initialize each sensor and return the results of the initialization
   */
  
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

  // GLOBAL LED DRIVER COMMANDS
  
  //set faderate
  I2CWRITE2BYTES_1(ADDRESS,FADERATE,0xEA6);  //
  I2CWRITE2BYTES_2(ADDRESS,FADERATE,0xEA6);
  I2CWRITE2BYTES_3(ADDRESS,FADERATE,0xEA6);
  I2CWRITE2BYTES_4(ADDRESS,FADERATE,0xEA6);
  
  //set offtime (this may have to be variable)
  
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_RED, 0x10);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_GREEN, 0x10);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_BLUE, 0x10);
  /*
  I2CWRITE2BYTES_2(ADDRESS,OFFTIME_RED, 0x20);
  I2CWRITE2BYTES_2(ADDRESS,OFFTIME_GREEN, 0x20);
  I2CWRITE2BYTES_2(ADDRESS,OFFTIME_BLUE, 0x20);
   
  I2CWRITE2BYTES_3(ADDRESS,OFFTIME_RED, 0x20);
  I2CWRITE2BYTES_3(ADDRESS,OFFTIME_GREEN, 0x20);
  I2CWRITE2BYTES_3(ADDRESS,OFFTIME_BLUE, 0x20);
  
  I2CWRITE2BYTES_4(ADDRESS,OFFTIME_RED, 0x20);
  I2CWRITE2BYTES_4(ADDRESS,OFFTIME_GREEN, 0x20);
  I2CWRITE2BYTES_4(ADDRESS,OFFTIME_BLUE, 0x20);
  */
  //set walktime
  I2CWRITE2BYTES_1(ADDRESS,WALKTIME,0x186);
  I2CWRITE2BYTES_2(ADDRESS,WALKTIME,0x186);
  I2CWRITE2BYTES_3(ADDRESS,WALKTIME,0x186);
  I2CWRITE2BYTES_4(ADDRESS,WALKTIME,0x186);
  
  //myWire1.printStatus(Serial);
}

void loop() {

  /*
   * Variable State LED Driver commands, state is controlled by isrSector* commands
  */

  Serial.println(i_1);
  Serial.println(red_int_val[i_1]);
  Serial.println(green_int_val[i_1]);
  Serial.println(blue_int_val[i_1]);
  Serial.println(dimming_array[i_1]);
  Serial.println(current[i_1]);
  Serial.println(offtime[i_1]);
  
  // SECTOR 1
  
  I2CWRITE2BYTES_1(ADDRESS,DIMMINGLEVEL,dimming_array[i_1]);
  //I2CWRITE2BYTES_1(ADDRESS,CURRENT_RED, current[i_1]);
  //I2CWRITE2BYTES_1(ADDRESS,CURRENT_BLUE, current[i_1]);
  //I2CWRITE2BYTES_1(ADDRESS,CURRENT_GREEN, current[i_1]);
  //I2CWRITE2BYTES_1(ADDRESS,OFFTIME_RED, offtime[i_1]);
  //I2CWRITE2BYTES_1(ADDRESS,OFFTIME_BLUE, offtime[i_1]);
  //I2CWRITE2BYTES_1(ADDRESS,OFFTIME_GREEN, offtime[i_1]);
  delay(100);
  SetIntensityRGB_1(red_int_val[i_1],green_int_val[i_1],blue_int_val[i_1]);
   
  // SECTOR 2
  SetIntensityRGB_2(red_int_val[i_2],blue_int_val[i_2],green_int_val[i_2]);
  I2CWRITE2BYTES_2(ADDRESS,DIMMINGLEVEL,dimming_array[i_2]);
  I2CWRITE2BYTES_2(ADDRESS,CURRENT_RED, current[i_2]);
  I2CWRITE2BYTES_2(ADDRESS,CURRENT_BLUE, current[i_2]);
  I2CWRITE2BYTES_2(ADDRESS,CURRENT_GREEN, current[i_2]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_RED, offtime[i_2]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_BLUE, offtime[i_2]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_GREEN, offtime[i_2]);
   
  // SECTOR 3
  SetIntensityRGB_3(red_int_val[i_3],blue_int_val[i_3],green_int_val[i_3]);
  I2CWRITE2BYTES_3(ADDRESS,DIMMINGLEVEL,dimming_array[i_3]);
  I2CWRITE2BYTES_3(ADDRESS,CURRENT_RED, current[i_3]);
  I2CWRITE2BYTES_3(ADDRESS,CURRENT_BLUE, current[i_3]);
  I2CWRITE2BYTES_3(ADDRESS,CURRENT_GREEN, current[i_3]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_RED, offtime[i_3]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_BLUE, offtime[i_3]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_GREEN, offtime[i_3]);
  
  // SECTOR 4
  SetIntensityRGB_4(red_int_val[i_4],blue_int_val[i_4],green_int_val[i_4]);
  I2CWRITE2BYTES_4(ADDRESS,DIMMINGLEVEL,dimming_array[i_4]);
  I2CWRITE2BYTES_4(ADDRESS,CURRENT_RED, current[i_4]);
  I2CWRITE2BYTES_4(ADDRESS,CURRENT_BLUE, current[i_4]);
  I2CWRITE2BYTES_4(ADDRESS,CURRENT_GREEN, current[i_4]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_RED, offtime[i_4]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_BLUE, offtime[i_4]);
  I2CWRITE2BYTES_1(ADDRESS,OFFTIME_GREEN, offtime[i_4]);  

  //TEST SCRIPT ONLY
  i_1++;
  i_2++;
  i_3++;
  i_4++;

  if(i_1 == 24){i_1=0;}
  if(i_2 == 24){i_2=0;}
  if(i_3 == 24){i_3=0;} 
  if(i_4 == 24){i_4=0;}  
  delay(2500);
  //Serial.println(i_1);
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
