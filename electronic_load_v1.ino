#define INPUT_VOLTAGE 2
int value;

void setup() {
  Serial.begin(9600);
  
  //Define signals for interfacing
  pinMode(2, OUTPUT);
}

void loop() {

  if(Serial.available() > 0){
    value = Serial.read();

    Serial.print(value);

    if(value == 1){
      digitalWrite(INPUT_VOLTAGE, HIGH);
    }else {
      digitalWrite(INPUT_VOLTAGE,LOW);
    }
  }
}
