// Define pins for digital IR sensors
const int irSensorLeft = A0;
const int irSensorRight = A1;
const int irSensorPerson = 12;

// Define pins for motor driver
const int motorLeftPin1 = 5;
const int motorLeftPin2 = 6;
const int motorRightPin1 = 9;
const int motorRightPin2 = 10;
const int motorLeftEnable = 3;
const int motorRightEnable = 11;

// Define pins for ultrasonic sensor
const int ultrasonicTrig = A3;
const int ultrasonicEcho = A4;

// Define pin for water level sensor
const int waterLevelSensor = A2;

// Define pin for pump
const int pumpPin = 8;

// Define variables for ultrasonic sensor
long duration;
int distance;

// Define variables for water level sensor
const int waterLevelThreshold = 500; // Adjust this value based on your sensor readings

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Set up pins for digital IR sensors as inputs
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorRight, INPUT);
  pinMode(irSensorPerson, INPUT);

  // Set up pins for motor driver as outputs
  pinMode(motorLeftPin1, OUTPUT);
  pinMode(motorLeftPin2, OUTPUT);
  pinMode(motorRightPin1, OUTPUT);
  pinMode(motorRightPin2, OUTPUT);
  pinMode(motorLeftEnable, OUTPUT);
  pinMode(motorRightEnable, OUTPUT);

  // Set up pins for ultrasonic sensor as outputs and inputs
  pinMode(ultrasonicTrig, OUTPUT);
  pinMode(ultrasonicEcho, INPUT);

  // Set up pin for water level sensor as input
  pinMode(waterLevelSensor, INPUT);

  // Set up pin for pump as output
  pinMode(pumpPin, OUTPUT);
  delay(30);
}

void loop() {
  // Read values from digital IR sensors
  int irLeftValue = digitalRead(irSensorLeft);
  int irRightValue = digitalRead(irSensorRight);
  int irPersonValue = digitalRead(irSensorPerson);

  // Read value from water level sensor
  int waterLevelValue = analogRead(waterLevelSensor);

  // Check if person is detected
  if (irPersonValue == HIGH) {
    //Serial.println("Person detected, stopping vehicle");
    // Stop the vehicle
    //stopVehicle();
    // Check for glass presence with ultrasonic sensor
    digitalWrite(ultrasonicTrig, LOW);
    delayMicroseconds(5);
    digitalWrite(ultrasonicTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonicTrig, LOW);

    duration = pulseIn(ultrasonicEcho, HIGH);
    distance = duration * 0.034 / 2;
    // Serial.print("Distance to glass: ");
    // Serial.println(distance);

    if (distance < 8 && distance > 0) { // Glass detected at 8 cm
      Serial.println("Glass detected, turning pump on");
      //stopVehicle();
      // Turn on pump
      digitalWrite(pumpPin, LOW);
      // Wait for the water level to reach threshold
      while (analogRead(waterLevelSensor) < waterLevelThreshold) {
        // Keep pump on
        stopVehicle();
      }
      Serial.println("Water level reached, turning pump off");
      
    }
    else{
    // Turn off pump
      digitalWrite(pumpPin, HIGH);
    }
  } 
  
    // Move the vehicle based on line tracking
    if (irLeftValue == HIGH && irRightValue == HIGH) {
      Serial.println("Both sensors on black line, going straight");
      // Both sensors on black line, go straight
      forward();
    } else if (irRightValue == HIGH) {
      Serial.println("Right sensor on black line, turning right");
      // Left sensor on black line, turn right
      turnRight();
    } else if (irLeftValue == HIGH) {
      Serial.println("Left sensor on black line, turning left");
      // Right sensor on black line, turn left
      turnLeft();
    } else if (irLeftValue == LOW && irRightValue == LOW){
      Serial.println("Both sensors off the line, stopping");
      // Both sensors off the line, stop
      stopVehicle();
    }
  
}

void forward() {
  Serial.println("Moving forward");
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftEnable, HIGH);
  digitalWrite(motorRightEnable, HIGH);
}

void turnLeft() {
  Serial.println("Turning left");
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, LOW);
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftEnable, HIGH);
  digitalWrite(motorRightEnable, HIGH);
}

void turnRight() {
  Serial.println("Turning right");
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
  digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftEnable, HIGH);
  digitalWrite(motorRightEnable, HIGH);
}

void stopVehicle() {
  Serial.println("Stopping vehicle");
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, LOW);
  digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftEnable, LOW);
  digitalWrite(motorRightEnable, LOW);
}
