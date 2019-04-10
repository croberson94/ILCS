
#include <ISL29125_SoftWire.h>
#include <Wire.h>
#include <RGBLEDLighting.h>

#define ADDRESS   0x150

InfineonRGB LEDS;  // Create Object



/*
 *Declare All Global Variables
 */

// RGB Values for light output cycle
const unsigned int day_int_val[3] = {0x555,0x555,0x555};
const unsigned int morning_int_val[3] = {0x555,0x555,0x555};
const unsigned int evening_int_val[3] = {0x555,0x555,0x555};
const unsigned int night_int_val[3] = {0x0,0x0,0x0};

//increment value
unsigned int i = 0;
byte received;
byte code;
byte offset;
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


/*
 * This function takes the byte sent by the GUI and
 * either extracts the offset or simply increments i
 */

void GUIhandler(int howMany){
  code = Wire.read();
  // change or initialize sector1 light fixture
  if(code == cyc_status1){
    i++;
    if(i == 24){i=0;}
  }
  else{
    //decode instructions in variable code
    cyc_status1 = code;
    offset = code & 0xF;
    i = 20 - int(offset);
    if(i == 24){i=0;}
    
  }
}


void setup() {
  Serial.begin(115200);
  //Initialize I2C bus for 4Duino Communication
  Wire.begin(8);
  Wire.onReceive(GUIhandler);
  
 
  // GLOBAL LED DRIVER COMMANDS
    int glob_faderate = 0xEA6;
		int glob_walktime = 0x186;
		int read_faderate = 0;
		int read_walktime = 0;
  //set faderate
  LEDS.I2CWRITE2BYTES(ADDRESS,FADERATE,glob_faderate); 
  //set walktime
  LEDS.I2CWRITE2BYTES(ADDRESS,WALKTIME,glob_walktime);
		
  read_walktime = LEDS.I2CREAD(ADDRESS, READ_WALKTIME);
	read_faderate = LEDS.I2CREAD(ADDRESS, READ_FADERATE);

  if(read_walktime == glob_walktime){
    if(read_faderate ==  glob_faderate){
      Serial.println("SECTOR # LED Driver Successfully initialized");
    }
    else{Serial.println("FADERATE initialization FAILED");}
  }
  else{Serial.println("WALKTIME initialization FAILED");} 
}


void loop() {

  /*
   * Variable State LED Driver commands, state is controlled by isrSector* commands
  */

  
  Serial.print("The byte sent equals ");
  Serial.println(code);
  // First thing we check is to see if the light should be off
  if(code == 192){
    LEDS.SetIntensityRGB(0x0,0x0,0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS,DIMMINGLEVEL,0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_RED, 0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_BLUE, 0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_GREEN, 0x0);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_RED, 0xFF);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_BLUE, 0xFF);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_GREEN, 0xFF);
    Serial.println("LIGHT IS OFF");
  }
  else{
  // SECTOR 1
    LEDS.SetIntensityRGB(red_int_val[i],red_int_val[i],red_int_val[i]);
    LEDS.I2CWRITE2BYTES(ADDRESS,DIMMINGLEVEL,dimming_array[i]);
    LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_RED, current[i]);
    LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_BLUE, current[i]);
    LEDS.I2CWRITE2BYTES(ADDRESS,CURRENT_GREEN, current[i]);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_RED, offtime[i]);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_BLUE, offtime[i]);
    LEDS.I2CWRITE2BYTES(ADDRESS,OFFTIME_GREEN, offtime[i]);
    Serial.println("LIGHT IS ON");
  }
  
  //TEST SCRIPT ONLY
  //i++;
  //if(i == 24){i=0;}
    
  delay(2500);
  Serial.print("The offset is ");
  Serial.println(offset);
 
  
}
