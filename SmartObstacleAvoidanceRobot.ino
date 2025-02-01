#define BLYNK_TEMPLATE_ID "TMPL60swo6lud"         // Your Blynk Template ID
#define BLYNK_DEVICE_NAME "IoT Based Smart Obstacle Avoidance Robot" // Your Device Name
#define BLYNK_AUTH_TOKEN "_5F9qtjyln2iqiKb-GzBEKrS0mlB-Q6r" // Your Auth Token

#include <WiFi.h>                // Wi-Fi library for Arduino Uno R4 Wi-Fi (ESP32-based)
#include <BlynkSimpleWifi.h>     // Blynk library for ESP32
#include <Servo.h>               // Servo library

// Wi-Fi credentials
const char* ssid = "MOYE MOYE";  // Wi-Fi network name
const char* password = "SABTHIKHOJAYEGA";      // Wi-Fi password

// Ultrasonic sensor pins
const int trigPin = 12;
const int echoPin = 13;

// Motor control pins
const int motorPin1 = 5;
const int motorPin2 = 6;
const int motorPin3 = 7;
const int motorPin4 = 8;

// Servo control pins
Servo servoUpDown, servoClaw;

// Distance calculation function
float checkDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) / 58.0; // Distance in cm
}

// Movement functions
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void moveLeft() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void moveRight() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stopMovement() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void setup() {
  // Serial communication
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting Smart Obstacle Avoidance Robot...");
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // Wait for connection
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    retryCount++;

    // If it takes too long, stop trying
    if (retryCount > 15) { // Waits for ~15 seconds
      Serial.println("\nFailed to connect to Wi-Fi. Check credentials and signal strength.");
      return;
    }
  }
  
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.println("Connected to Blynk!");

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Attach servos
  servoUpDown.attach(9); // Arm servo
  servoClaw.attach(10);  // Claw servo

  // Blynk initialization
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run(); // Keep the Blynk connection active

  // Send distance to Blynk (Virtual Pin V7)
  float distance = checkDistance();
  Blynk.virtualWrite(V7, distance);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000); // Adjust as needed
}

// Blynk Virtual Pin Functions for movement and servo controls
BLYNK_WRITE(V1) { // Forward movement
  int pinValue = param.asInt();
  if (pinValue == 1) moveForward();
  else stopMovement();
}

BLYNK_WRITE(V2) { // Backward movement
  int pinValue = param.asInt();
  if (pinValue == 1) moveBackward();
  else stopMovement();
}

BLYNK_WRITE(V3) { // Left movement
  int pinValue = param.asInt();
  if (pinValue == 1) moveLeft();
  else stopMovement();
}

BLYNK_WRITE(V4) { // Right movement
  int pinValue = param.asInt();
  if (pinValue == 1) moveRight();
  else stopMovement();
}

BLYNK_WRITE(V5) { // Claw servo control
  int angle = param.asInt();
  servoClaw.write(angle);
}

BLYNK_WRITE(V6) { // Up-down servo control
  int angle = param.asInt();
  servoUpDown.write(angle);
}