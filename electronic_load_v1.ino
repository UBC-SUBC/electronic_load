#include <ArduinoJson.h>

#define INPUT_VOLTAGE 2
int value;



void setup() {
  Serial.begin(9600);
  
  //Define signals for interfacing
  pinMode(2, OUTPUT);
}

void loop() {
  StaticJsonDocument<200> jsonBuffer;
  
  if(Serial.available() > 0){
    
    DeserializationError err = deserializeJson(jsonBuffer, input);

    if(value == '1'){
      digitalWrite(INPUT_VOLTAGE, HIGH);
    }else {
      digitalWrite(INPUT_VOLTAGE,LOW);
    }
  }
}
