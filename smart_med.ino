#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

const int ledPin = 12;    // Pin connected to the LED
const int buzzerPin = 8;  // Pin connected to the Buzzer
const int servoPin = 9;   // Pin connected to the Servo

RTC_DS3231 rtc;           // Create an RTC object
Servo myServo;            // Create a Servo object

bool activated = false;   // Flag to track if the LED, Buzzer, and Servo have been activated
unsigned long activationTime = 0; // Variable to store the activation time
bool servoMoved = false;  // Flag to ensure the servo motor moves only once

void setup() {
  Serial.begin(9600); // Start serial communication for debugging
  
  // Initialize the RTC module
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC lost power and if so, set the time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    // Set the date and time to the current date and time manually
    // Set to 31st July 2024, 10:40:00 for example
    rtc.adjust(DateTime(2024, 7, 31, 10, 40, 0)); 
  }

  pinMode(ledPin, OUTPUT);    // Set the LED pin as an output
  pinMode(buzzerPin, OUTPUT); // Set the Buzzer pin as an output
  myServo.attach(servoPin);   // Attach the Servo control pin
}

void loop() {
  DateTime now = rtc.now();  // Get the current time

  // Print the current time to the serial monitor
  Serial.print(now.hour(), DEC);
  Serial.print(':');  
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // Set the desired time to turn the LED, Buzzer, and Servo on
  int onHour = 10;  // Hour to activate (24-hour format)
  int onMinute = 45; // Minute to activate

  // Activate the LED, Buzzer, and Servo for 30 seconds if the current time matches the activation time
  if (now.hour() == onHour && now.minute() == onMinute && !activated) {
    digitalWrite(ledPin, HIGH);    // Turn on the LED
    digitalWrite(buzzerPin, HIGH); // Turn on the Buzzer
    activated = true;
    activationTime = millis();     // Record the activation time

    // Move the servo to 50 degrees and immediately back to 0 degrees
    if (!servoMoved) {
      myServo.write(50);           // Move the servo to 50 degrees
      delay(1000);                 // Give the servo time to move
      myServo.write(0);            // Return the servo to 0 degrees
      servoMoved = true;
    }
  }

  // Turn off the LED and Buzzer after 30 seconds of activation
  if (activated && (millis() - activationTime >= 30000)) {
    digitalWrite(ledPin, LOW);    // Turn off the LED
    digitalWrite(buzzerPin, LOW); // Turn off the Buzzer
    activated = false;            // Reset the activation flag
    servoMoved = false;           // Reset the servo move flag for next activation
  }

  delay(1000);  // Wait for 1 second before checking the time again
}
