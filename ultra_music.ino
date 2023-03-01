/* 
ultra_music: Code for an Arduino-based, customisable electronic musical instrument.
The code runs on an Arduino Uno connected to an ultrasonic range sensor and a potentiometer.
Signals from these sensors are sent via USB, which can be used to control different musical tones.
*/

// Pins on the Arduino connected to the ultrasonic range sensor
#define TRIGGERPIN 10
#define ECHOPIN 9

// Minimum and maximum distance readings (in mm) for controlling sounds
#define MIN_DISTANCE 30.0
#define MAX_DISTANCE 500.0

// This function is called once at the start of the program
void setup() {
  pinMode(TRIGGERPIN, OUTPUT); // Pulses will be sent out of the Arduino
  pinMode(ECHOPIN, INPUT); // Echo pulses will be received into the Arduino
  Serial.begin(115200);
}

// This function is called repeatedly by the program
void loop() {

  // Read current potentiometer value
  int sensor_value = analogRead(A0);

  // Send an output pulse to the sonar sensor (1ms wide)
  digitalWrite(TRIGGERPIN, HIGH);
  delay(1); // wait 1ms
  digitalWrite(TRIGGERPIN, LOW);

  // Measure the time taken to receive the pulse back // Will be "0" or "-1" if no pulse is received
  int duration = pulseIn(ECHOPIN, HIGH);

  // Formula for the distance is half the duration, then divide by 2.910
  // to get distance in mm
  int distance = (duration/2) / 2.91;

  // Convert this into a control signal between 0 and 1023
  int distance_value;
  if ( (distance < MIN_DISTANCE) || (distance > MAX_DISTANCE) ) {
    distance_value = -1;
  }
  else {
    float df = (distance-MIN_DISTANCE)/(MAX_DISTANCE-MIN_DISTANCE);
    distance_value = df*1024; // "distance_value" is between 0 (MIN) and 1023 (MAX)
  }

  // Send message across USB
  Serial.print(distance_value);
  Serial.print(' ');
  Serial.print(sensor_value);
  Serial.println("");
  Serial.flush();

  // wait 50 milliseconds between messages sent
  delay(50);

}
