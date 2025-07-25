#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Joysticks
const int joy1XPin = A0;
const int joy1YPin = A1; 
const int joy2XPin = A2; 
const int joy2YPin = A3; 

RF24 radio(7, 8);

const byte address[6] = "00001";

struct Data {
  int joy1X;
  int joy1Y;
  int joy2X;
  int joy2Y;
};

Data data;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

  data.joy1X = analogRead(joy1XPin);
  data.joy1Y = analogRead(joy1YPin);
  data.joy2X = analogRead(joy2XPin);
  data.joy2Y = analogRead(joy2YPin);

  radio.write(&data, sizeof(data));

  delay(50); 
}
