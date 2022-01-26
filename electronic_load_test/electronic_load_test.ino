#include <ArduinoJson.h> //Using Version 5.13.5

#define INPUT_VOLTAGE 2
int value;
String txt;



void setup() {
  Serial.begin(9600);
  
  //Define signals for interfacing
  pinMode(2, OUTPUT);
}

void loop() {
  StaticJsonBuffer<200> jsonBuffer;
  
 if(Serial.available() > 0){
      
    txt = Serial.readString();
    //txt = "{\"value\":\"1\"}";
    JsonObject& root = jsonBuffer.parseObject(txt);

    //Check if the json was decoded properly
    if(!root.success()){
      Serial.println("parseObject() failed");
      return false;
    }
    else{
      value = (int) root["value"];
      //value = 1;
      
      if(value == 1){
        digitalWrite(INPUT_VOLTAGE, HIGH);
      }else {
        digitalWrite(INPUT_VOLTAGE,LOW);
      }
    }
  }
  Serial.print(value);
}
