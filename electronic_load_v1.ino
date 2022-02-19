#include <ArduinoJson.h> //Using Version 5.13.5

#define INPUT_VOLTAGE 2
int power;
int current;
int voltage;
bool mode;

char txt[200];

void setup() {
  Serial.begin(9600);
  
  //Define signals for interfacing
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.print("Initial Message\n");
}

void loop() {
  StaticJsonBuffer<200> jsonBuffer;
  
 if(Serial.available() > 0){
    
    Serial.readBytesUntil('\n', txt, 200);
    //txt = "{\"value\":\"1\"}";
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

      //Need to set voltage between 0-5 volts
      if(mode){
          digitalWrite(INPUT_VOLTAGE, HIGH);
          root["power"] = 1; //For testing
          root.printTo(Serial);
          Serial.print("\n"); //Try to do the line endings thing so that the code is more responsive and we don't need to specify a size to read
      }
      else{
        digitalWrite(INPUT_VOLTAGE, LOW);
       root["current"] = "100"; //For testing
       root.printTo(Serial);
       Serial.print("\n");
      }
    }
  }
  //Serial.print("Buffer");
}
