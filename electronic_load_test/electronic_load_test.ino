#include <ArduinoJson.h>

#define INPUT_VOLTAGE 2
int value;
char txt[200];


void setup() {
  Serial.begin(9600);
  
  //Define signals for interfacing
  pinMode(2, OUTPUT);
}

void loop() {
  StaticJsonDocument<200> jsonBuffer;
  
  if(Serial.available() > 0){
    
    for(int i = 0; i<200; i++){
      txt[i] = (char) Serial.read();
    }
    
    DeserializationError err = deserializeJson(jsonBuffer, txt);

    //Check if the json was deserialized properly
    if(err){
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
    }

    value = (int) jsonBuffer["value"];
    Serial.print(value);
    
    if(value == 1){
      digitalWrite(INPUT_VOLTAGE, HIGH);
    }else {
      digitalWrite(INPUT_VOLTAGE,LOW);
    }
  }
}
