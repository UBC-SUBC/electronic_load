#include<Wire.h> 
#define MCP4725 0x60 //Not sure if this should be 0x60, 0x61 or 0x62

unsigned int DAC_in;
byte buffer[3];
double DAC_out;

void setup() {
  Wire.begin();
  //pinMode(A1, INPUT);
  Serial.begin(115200);
}

void loop() {
  buffer[0] = 0b01000000; 

  DAC_in = ;
  
  buffer[1] = DAC_in >> 4;              //Puts the most significant bit values
  buffer[2] = DAC_in << 4;              //Puts the Least significant bit values

  Wire.beginTransmission(MCP4725);
  Wire.write(buffer[0]);
  Wire.write(buffer[1]);
  Wire.write(buffer[2]);
  Wire.endTransmission();

  DAC_out = 5.0*analogRead(A1)/1023.0;

  Serial.println("DAC_out: ");
  Serial.println(DAC_out);
  
}
