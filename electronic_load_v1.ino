#include <ArduinoJson.h> //Using Version 5.13.5
#include<Wire.h> 
#define MCP4725 0x60 //MCP Address is 0x60 because our board has A0 grounded

#define VOUT A1
#define MOSFET_DR A2

//DAC Variables
unsigned int DAC_in;
double DAC_out; //Make sure DAC_out is a double so no values get truncated
float voltage_in;
float mosfet_drain;
float shunt_voltage;
byte buffer[3];

//MATLAB Variables
int power;
int current;
int voltage_read;
bool mode;

//Buffer for reading JSON-encoded data from MATLAB
char txt[200];

void setup() {
  Serial.begin(9600); //Could potentially be faster??

  //Setup transmission to DAC
  Wire.begin();
   
  //Define signals for interfacing
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  //Send an initial message to the serial port so MATLAB doesn't hang
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

      //Read the MOSFET drain voltage
      mosfet_drain = 5.0*analogRead(MOSFET_DR)/1023.0; //Arduino ADC is only 10-bits

      //Make a guess for the input voltage that should be provided
      //Power-controlled mode
      if(!mode){
        voltage_in = (power/mosfet_drain)*101/100; //Assume voltage across MOSFET is constant to simplify calculations. From circuit Vin = IDS*101/100
      }
      //Current-controlled mode
      else{
        //Set input voltage based on IDS & Voltage Divider
        voltage_in = current*101/100; 
      }

      //Send the voltage to the circuit and adjust until the current reaches the desired value.
      do{
        //Send the control byte first, so that the DAC knows communciation is beginning
        buffer[0] = 0b01000000; 

        //Convert desired voltage to a number that the DAC can understand
        DAC_in = (voltage_in/5.0)*(4096.0);
  
        buffer[1] = DAC_in >> 4;              //Prepare the most significant bit values
        buffer[2] = DAC_in << 4;              //Prepare the least significant bit values

        //Send the data
        Wire.beginTransmission(MCP4725);
        Wire.write(buffer[0]);
        Wire.write(buffer[1]);
        Wire.write(buffer[2]);
        Wire.endTransmission();

        //Read the voltage across the shunt resistor
        DAC_out = 5.0*analogRead(VOUT)/1023.0; //Arduino ADC is only 10-bits
        shunt_voltage = DAC_out/100;
      } while(abs(shunt_voltage - voltage_in)> 0.1);

       root["voltage"] = mosfet_drain - shunt_voltage; //Drain-source voltage of MOSFET
       root["current"] = shunt_voltage/100; //MOSFET IDS equals current through shunt resistor. Shunt resistor is 0.01 ohms
       root["power"] = (float) root["current"] * (float) root["voltage"]; //Power through MOSFET

       //Print data to serial port
       root.printTo(Serial);
       Serial.print("\n");
      
    }
  }
  //Serial.print("Buffer");
}
