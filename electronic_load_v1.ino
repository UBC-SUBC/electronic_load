#include <ArduinoJson.h> //Using Version 5.13.5
#include<Wire.h> 
#define MCP4725 0x60 //MCP Address is 0x60 because our board has A0 grounded

//#define INPUT_VOLTAGE 2

//DAC Variables
unsigned int DAC_in;
double DAC_out; //Make sure DAC_out is a double so no values get truncated
byte buffer[3];

//MATLAB Variables
int power;
int current;
int voltage;
bool mode;

//Buffer for reading JSON-encoded data from MATLAB
char txt[200];

void setup() {
  Serial.begin(9600); //Could potentially be faster??

   Wire.begin();
   
  //Define signals for interfacing
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.print("Initial Message\n");
}

void loop() {
  StaticJsonBuffer<200> jsonBuffer;

  //Check if serial port is available
 if(Serial.available() > 0){

    //Read the data just sent from MATLAB. Data must be terminated by newline character
    Serial.readBytesUntil('\n', txt, 200);
    //Parse the string just read
    JsonObject& root = jsonBuffer.parseObject(txt);

    //Check if the json was decoded properly
    if(!root.success()){
      Serial.println("parseObject() failed");
      digitalWrite(13, HIGH);  
    }
    else{
      //Extract info from decoded json
      power = (int) root["power"];
      current = (int) root["current"];
      mode = (bool) root ["mode"];

      buffer[0] = 0b01000000; //Control byte for I2C communication

      //Send input voltage to DAC
      if(!mode){
          //Testing to see if DAC works with MATLAB Code
          DAC_in = power*50; //Max power is 80, so 80*50=4000 which is close to the maximum value the DAC can comprehend
 
          root["power"] = analogRead(A1)/50; //Testing to see if Arduino ADC works with MATLAB
          root["voltage"] = 5;
          root.printTo(Serial); //Send data back to MATLAB
          Serial.print("\n"); //Data must be terminated by a newline character
      }
      else{
       //Test to see if DAC works with MATLAB
       DAC_in = current*800; //Max current is 5A so 800*5 = 4000 which is close to the max value the DAC can comprehend

       root["current"] = analogRead(A1)/800;
       root.printTo(Serial);
       Serial.print("\n");
      }
    }
  }
  //Serial.print("Buffer");
}
