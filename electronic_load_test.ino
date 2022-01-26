#include <ArduinoJson.h> //Using Version 5.13.5

#define INPUT_VOLTAGE 2
int value;
char txt[200];



void setup() {
  Serial.begin(9600);
  
  //Define signals for interfacing
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
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
      //return false;
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
  Serial.print(txt);//value);
}
