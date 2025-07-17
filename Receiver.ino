#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8);

const byte address[6] = "00001";

const int motor1Pin1 = 10;
const int motor1Pin2 = 9;
const int motor2Pin1 = 4;
const int motor2Pin2 = 2;
const int enablePin1 = 6;
const int enablePin2 = 5;

const int servoPin = 3;

Servo servo;

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
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  servo.attach(servoPin);
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    int motorSpeedY = map(data.joy1Y, 0, 1023, -255, 255);
    int motorSpeedX = map(data.joy1X, 0, 1023, -255, 255);

    int motor1Speed = constrain(motorSpeedY + motorSpeedX, -255, 255);
    int motor2Speed = constrain(motorSpeedY - motorSpeedX, -255, 255);

    analogWrite(enablePin1, abs(motor1Speed));
    analogWrite(enablePin2, abs(motor2Speed));

    if (motor1Speed > 0) {
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
    } else {
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
    }

    if (motor2Speed > 0) {
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
    } else {
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
    }

    int servoPos = map(data.joy2X, 0, 1023, 0, 180);
    servo.write(servoPos);
  }
}
