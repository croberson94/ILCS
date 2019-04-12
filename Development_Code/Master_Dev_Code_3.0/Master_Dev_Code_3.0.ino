
#include <ISL29125_SoftWire.h>
#include <Wire.h>
#include <RGBLEDLighting.h>

#define ADDRESS1   0x100
#define ADDRESS2   0x152
#define ADDRESS3   0x154
#define ADDRESS4   0x156

InfineonRGB LEDS;  // Create Object

ISL29125_SOFT RGB_sensor[2]; // Create RGB Sensor Objects

/*
 *Declare All Global Variables
 */

// RGB Values for light output cycle
const unsigned int day_int_val[3] = {0xDD0,0xDD0,0xDDD};
const unsigned int morning_int_val[3] = {0x5FF,0x555,0x500};
const unsigned int evening_int_val[3] = {0x5DD,0x555,0x555};
const unsigned int night_int_val[3] = {0x0,0x0,0x0};

//increment value
unsigned int i_1 = 12;
unsigned int i_2 = 6;
unsigned int i_3 = 18;
unsigned int i_4 = 0;
byte received;
byte code;
byte offset;
// Cycle Status Variables
int cyc_status1 = 0xFF;

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


/*
 * This function takes the byte sent by the GUI and
 * either extracts the offset or simply increments i
 */

void GUIhandler(int howMany){
  code = Wire.read();
  // change or initialize sector1 light fixture
  if(code == cyc_status1){
    i_1++;
    if(i_1 == 24){i_1=0;}
  }
  else{
    //decode instructions in variable code
    cyc_status1 = code;
    offset = code & 0xF;
    i_1 = 20 - int(offset);
    if(i_1 == 24){i_1=0;}
    
  }
}

void Set_Current(int address,unsigned int red,unsigned int green, unsigned int blue){
    LEDS.I2CWRITE2BYTES(address,CURRENT_RED, red);
    LEDS.I2CWRITE2BYTES(address,CURRENT_GREEN, green);
    LEDS.I2CWRITE2BYTES(address,CURRENT_BLUE, blue);
}

void Set_Offtime(int address,unsigned int red, unsigned int green, unsigned int blue){
    LEDS.I2CWRITE2BYTES(address,OFFTIME_RED, red);
    LEDS.I2CWRITE2BYTES(address,OFFTIME_GREEN, green);
    LEDS.I2CWRITE2BYTES(address,OFFTIME_BLUE, blue);
}

void setup() {
  Serial.begin(115200);
  //Initialize I2C bus for 4Duino Communication
  Wire.begin(11);
  Wire.onReceive(GUIhandler);
  // SECTOR 1 LED DRIVER COMMANDS
    int glob_faderate = 0xEA6;
    int glob_walktime = 0x186;
    int read_faderate_1 = 0;
    int read_walktime_1 = 0;
  //set faderate
  LEDS.I2CWRITE2BYTES(ADDRESS1,FADERATE,glob_faderate); 
  //set walktime
  LEDS.I2CWRITE2BYTES(ADDRESS1,WALKTIME,glob_walktime);
    
  read_walktime_1 = LEDS.I2CREAD(ADDRESS1, READ_WALKTIME);
  read_faderate_1 = LEDS.I2CREAD(ADDRESS1, READ_FADERATE);

  if (read_walktime_1 == glob_walktime){
      Serial.println("SECTOR 1 WALKTIME Successfully initialized");
  }
  else {Serial.println("SECTOR 1 WALKTIME initialization FAILED");}
  delay(100);
  if (read_faderate_1 ==  glob_faderate){
      Serial.println("SECTOR 1 FADERATE Successfully initialized");
  }
  else {Serial.println("SECTOR 1 FADERATE initialization FAILED");} 
  delay(100);
  // SECTOR 2 LED DRIVER COMMANDS
    int read_faderate_2 = 0;
    int read_walktime_2 = 0;
  //set faderate
  LEDS.I2CWRITE2BYTES(ADDRESS2,FADERATE,glob_faderate); 
  //set walktime
  LEDS.I2CWRITE2BYTES(ADDRESS2,WALKTIME,glob_walktime);
    
  read_walktime_2 = LEDS.I2CREAD(ADDRESS2, READ_WALKTIME);
  read_faderate_2 = LEDS.I2CREAD(ADDRESS2, READ_FADERATE);

  if (read_walktime_2 == glob_walktime){
      Serial.println("SECTOR 2 WALKTIME Successfully initialized");
  }
  else {Serial.println("SECTOR 2 WALKTIME initialization FAILED");}
  delay(100);
  if (read_faderate_2 ==  glob_faderate){
      Serial.println("SECTOR 2 FADRATE Successfully initialized");
  }
  else {Serial.println("SECTOR 2 FADERATE initialization FAILED");} 
  delay(100);
 // SECTOR 3 LED DRIVER COMMANDS
    int read_faderate_3 = 0;
    int read_walktime_3 = 0;
  //set faderate
  LEDS.I2CWRITE2BYTES(ADDRESS3,FADERATE,glob_faderate); 
  //set walktime
  LEDS.I2CWRITE2BYTES(ADDRESS3,WALKTIME,glob_walktime);
    
  read_walktime_3 = LEDS.I2CREAD(ADDRESS3, READ_WALKTIME);
  read_faderate_3 = LEDS.I2CREAD(ADDRESS3, READ_FADERATE);

  if (read_walktime_3 == glob_walktime){
      Serial.println("SECTOR 3 WALKTIME Successfully initialized");
  }
  else {Serial.println("SECTOR 3 WALKTIME initialization FAILED");}
  delay(100);
  if (read_faderate_3 ==  glob_faderate){
      Serial.println("SECTOR 3 FADRATE Successfully initialized");
  }
  else {Serial.println("SECTOR 3 FADERATE initialization FAILED");}  
  delay(100);
  // SECTOR 4 LED DRIVER COMMANDS
		int read_faderate_4 = 0;
		int read_walktime_4 = 0;
  //set faderate
  LEDS.I2CWRITE2BYTES(ADDRESS4,FADERATE,glob_faderate); 
  //set walktime
  LEDS.I2CWRITE2BYTES(ADDRESS4,WALKTIME,glob_walktime);
		
  read_walktime_4 = LEDS.I2CREAD(ADDRESS4, READ_WALKTIME);
	read_faderate_4 = LEDS.I2CREAD(ADDRESS4, READ_FADERATE);

  if (read_walktime_4 == glob_walktime){
      Serial.println("SECTOR 4 WALKTIME Successfully initialized");
  }
  else {Serial.println("SECTOR 4 WALKTIME initialization FAILED");}
  delay(100);
  if (read_faderate_4 ==  glob_faderate){
      Serial.println("SECTOR 4 FADRATE Successfully initialized");
  }
  else {Serial.println("SECTOR 4 FADERATE initialization FAILED");} 
  delay(100);


// Initialize the ISL29125 with simple configuration so it starts sampling (Pin 2 is SDA, pin 3 is SCL)
  if (RGB_sensor[0].init(2,3)) 
  {
    Serial.println("Sensor_1 Initialization Successful\n\r");
  }
  else {
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
void loop() {

  /*
   * Variable State LED Driver commands, state is controlled by isrSector* commands
  */

  
  Serial.print("The byte sent equals ");
  Serial.println(code);
   /*
   * SECTOR 1 COMMANDS
   */
  // First thing we check is to see if the light should be off
  if(code == 192){
    /*
    LEDS.I2CWRITE6BYTES(ADDRESS1,INTENSITY_RGB,0x0,0x0,0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS1,DIMMINGLEVEL,0x0);
    Set_Current(ADDRESS1,0x0,0x0,0x0);
    Set_Offtime(ADDRESS1,0xFF,0xFF,0xFF);
    Serial.println("LIGHT IS OFF");
    */
  }
  else{
    LEDS.I2CWRITE6BYTES(ADDRESS1,INTENSITY_RGB,red_int_val[i_1],green_int_val[i_1],blue_int_val[i_1]);
    LEDS.I2CWRITE2BYTES(ADDRESS1,DIMMINGLEVEL,dimming_array[i_1]);
    Set_Current(ADDRESS1,current[i_1],current[i_1],current[i_1]);
    Set_Offtime(ADDRESS1,offtime[i_1],offtime[i_1],offtime[i_1]);
    Serial.println("LIGHT IS ON");
    
  }

  
  /*
   * SECTOR 2 COMMANDS
   */
  // First thing we check is to see if the light should be off
  if(code == 192){
    
    LEDS.I2CWRITE6BYTES(ADDRESS2,INTENSITY_RGB,0x0,0x0,0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS2,DIMMINGLEVEL,0x0);
    Set_Current(ADDRESS2,0x0,0x0,0x0);
    Set_Offtime(ADDRESS2,0xFF,0xFF,0xFF);
    Serial.println("LIGHT IS OFF");
  }
  else{
    LEDS.I2CWRITE6BYTES(ADDRESS2,INTENSITY_RGB,red_int_val[i_2],green_int_val[i_2],blue_int_val[i_2]);
    LEDS.I2CWRITE2BYTES(ADDRESS2,DIMMINGLEVEL,dimming_array[i_2]);
    Set_Current(ADDRESS1,current[i_2],current[i_2],current[i_2]);
    Set_Offtime(ADDRESS1,offtime[i_2],offtime[i_2],offtime[i_2]);
    Serial.println("LIGHT IS ON");
  }

  /*
   * SECTOR 3 COMMANDS
   */
  // First thing we check is to see if the light should be off
  if(code == 192){
    
    LEDS.I2CWRITE6BYTES(ADDRESS3,INTENSITY_RGB,0x0,0x0,0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS3,DIMMINGLEVEL,0x0);
    
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_RED, 0xFF);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_BLUE, 0xFF);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_GREEN, 0xFF);
    Serial.println("LIGHT IS OFF");
  }
  else{
    LEDS.I2CWRITE6BYTES(ADDRESS3,INTENSITY_RGB,red_int_val[i_3],green_int_val[i_3],blue_int_val[i_3]);
    LEDS.I2CWRITE2BYTES(ADDRESS3,DIMMINGLEVEL,dimming_array[i_3]);
    Set_Current(ADDRESS1,current[i_3],current[i_3],current[i_3]);
    Set_Offtime(ADDRESS1,offtime[i_3],offtime[i_3],offtime[i_3]);
    Serial.println("LIGHT IS ON");
  }
  /*
   * SECTOR 4 COMMANDS
   */
  // First thing we check is to see if the light should be off
  if(code == 192){
    
    LEDS.I2CWRITE6BYTES(ADDRESS4,INTENSITY_RGB,0x0,0x0,0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS4,DIMMINGLEVEL,0x0);
    
    LEDS.I2CWRITE2BYTES(ADDRESS4,OFFTIME_RED, 0xFF);
    LEDS.I2CWRITE2BYTES(ADDRESS4,OFFTIME_BLUE, 0xFF);
    LEDS.I2CWRITE2BYTES(ADDRESS4,OFFTIME_GREEN, 0xFF);
    Serial.println("LIGHT IS OFF");
  }
  else{
    LEDS.I2CWRITE6BYTES(ADDRESS4,INTENSITY_RGB,red_int_val[i_4],green_int_val[i_4],blue_int_val[i_4]);
    LEDS.I2CWRITE2BYTES(ADDRESS4,DIMMINGLEVEL,dimming_array[i_4]);
    Set_Current(ADDRESS1,current[i_4],current[i_4],current[i_4]);
    Set_Offtime(ADDRESS1,offtime[i_4],offtime[i_4],offtime[i_4]);
    Serial.println("LIGHT IS ON");
  }
  
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
  Serial.print("The offset is ");
  Serial.println(offset);

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
}
