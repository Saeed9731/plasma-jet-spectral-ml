//=======DALLAS setup========
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


//========DHT11setup=========

#include "DHT.h"
#define DHTPIN 3     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

//========addCTE=========
const int TEMP_THRESHOLD_UPPER = 37; // upper threshold of temperature, change to your desire value
const int HumIDITY_THRESHOLD_UPPER = 20; // upper threshold of HumIDITY, change to your desire value

const int RELAY_HOT_FAN_PIN = 4; // Arduino pin connected to relay which connected to fan
const int RELAY_COLD_FAN_PIN = 5; // Arduino pin connected to relay which connected to fan
const int RELAY_HumIDITY_FAN_PIN = 6; // Arduino pin connected to relay which connected to fan
const int PlasmaPin = 7; // the pin that the LED is attached to
const int Secondary_RELAY_HumIDITY = 8;
//.................................
int incomingByte;      // a variable to read incoming serial data into
int fanTemp; // automate control FAN with Temperature 
int PIZOHum; // automate control FAN with Temperature 

 
void setup(void)
{
  Serial.begin(9600);
  dht.begin();
  sensors.begin();
  pinMode(PlasmaPin, OUTPUT);
  pinMode(RELAY_HOT_FAN_PIN, OUTPUT); // initialize digital pin as an output
  pinMode(RELAY_COLD_FAN_PIN, OUTPUT); // initialize digital pin as an output
  // pinMode(RELAY_HumIDITY_FAN_PIN, OUTPUT); // initialize digital pin as an output
  // pinMode(Secondary_RELAY_HumIDITY, OUTPUT);
}

// //control humidity
// int acControlHumidity(float humidity, int HumIDITY_THRESHOLD_UPPER) {
//   Serial.println("The automated fan control based on humidity has been activated.");
//   delay(10000);
//   if(humidity <= TEMP_THRESHOLD_UPPER){
//     digitalWrite(RELAY_HumIDITY_FAN_PIN, HIGH); // turn off
//     digitalWrite(Secondary_RELAY_HumIDITY, HIGH); 
//     delay(100);
//     digitalWtite(Secondary_RELAY_HumIDITY, LOW); 
//   }
//   else{
//     digitalWrite(RELAY_HumIDITY_FAN_PIN, LOW); // turn on
//   }
//   return 0;
// }

// automate control FAN with Temperature 
int acControlTemperature(float dallas, int TEMP_THRESHOLD_UPPER) {
  Serial.println("The automated fan control based on temperature has been activated.");
  delay(10000);
  if(dallas <= TEMP_THRESHOLD_UPPER){
    Serial.println("The COLD fan is turned off");
    digitalWrite(RELAY_HOT_FAN_PIN, HIGH); // turn on
  }
  else{
    digitalWrite(RELAY_COLD_FAN_PIN, LOW); // turn on
    digitalWrite(RELAY_HOT_FAN_PIN, LOW); // turn on
  }
  return 0;
}

void loop() {

 //=========DHT setup=============
 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

//==========DALLAS setup============

// call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures

  float dallas = (sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire

  String data_humiditydht11  =     String(h);
  String data_thempdht11     =     String(t);
  String data_dallas         =     String(dallas);

  
  String temp1_humidity   = String("A" + data_humiditydht11);
  String temp2_dht11      = String("B" + data_thempdht11);
  String temp4_dallas     = String("D" + data_dallas);

  //.................................................//
  // upper threshold of temperature
  String data_upperTemp     =     String(TEMP_THRESHOLD_UPPER);
  String Hot_upperTemp   = String("U" + data_upperTemp);
  Serial.println(Hot_upperTemp); 

  // upper threshold of Humidity
  String data_upperHum     =     String(HumIDITY_THRESHOLD_UPPER);
  String Hig_upperHum  = String("M" + data_upperHum);
  Serial.println(Hig_upperHum);
  //.................................................//
  Serial.println(temp1_humidity); 
  Serial.println(temp2_dht11);
  Serial.println(temp4_dallas);
  //.................................................//
  // automate control FAN with Temperature 
  acControlTemperature(dallas, TEMP_THRESHOLD_UPPER);
  //.................................................//

  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    switch(incomingByte){
      //.......................................................
      // Temperature fan Contorol
      case 'C':
        Serial.println("The Upper fan is turned on");
        digitalWrite(RELAY_COLD_FAN_PIN, HIGH); // turn on
        break;

      case 'A':
        Serial.println("The Upper fan is turned off");
        digitalWrite(RELAY_COLD_FAN_PIN, LOW); // turn off
        break;
      //.......................................................
      // Plasma Gas Contorol
      case 'H':
      //enabled 
        Serial.println("Gas On");
        digitalWrite(PlasmaPin, HIGH);
        break;
      case 'L':
      //disabled
        Serial.println("Gas Off");
        digitalWrite(PlasmaPin, LOW);
        break;
      //.......................................................
      // // Humidity Contorol
      // case 'U':
      //   //enabled 
      //   Serial.println("The ControlHumidity is turned on");
      //   digitalWrite(RELAY_HumIDITY_FAN_PIN, HIGH); // turn on
      //   break;
      // case 'F':
      //   //disabled
      //   Serial.println("The ControlHumidity is turned off");
      //   digitalWrite(RELAY_HumIDITY_FAN_PIN, LOW); // turn off
      //   break;

      //.......................................................
    }
  }

  delay(250);
  
}
