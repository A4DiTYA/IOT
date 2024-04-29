#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include "MQ2.h" 


int Analog_Input = A0;
int lpg, co, smoke, smoke1;
 float h, t;

MQ2 mq2(Analog_Input);


const char *ssid =  "Galaxy A22 5G7259";  
const char *pass =  "tflp4120";
DHT dht(D5, DHT11);
WiFiClient client;
long myChannelNumber = 1498653;
const char myWriteAPIKey[] = "WIMEYLX14IIGH3I3";


// The load resistance on the board
//#define RLOAD 22.0
// Calibration resistance at atmospheric CO2 level
//#define RZERO 879.13 

int val; 

void setup() {
    mq2.begin(); 
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }

  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  dht.begin();
  ThingSpeak.begin(client);
}
void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  smoke1=(smoke*100)/1000000;
  
  Serial.println("LPG:");
 Serial.println(lpg);
 Serial.println(" CO:");
 Serial.println(co);

  Serial.println("SMOKE:");
  Serial.println(smoke1);
  Serial.println(" %");
  delay(1000);
 
  ThingSpeak.writeField(myChannelNumber, 1, lpg, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, co, myWriteAPIKey);
   ThingSpeak.writeField(myChannelNumber, 3, smoke1, myWriteAPIKey);
  Serial.println("Temperature: " + (String) t);
  Serial.println("Humidity: " + (String) h);
  ThingSpeak.writeField(myChannelNumber, 4, t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 5, h, myWriteAPIKey);
  delay(100);
}
