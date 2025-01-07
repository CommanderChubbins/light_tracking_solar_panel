#include <Servo.h>

#define SENSOR_MIN 0
#define SENSOR_MAX 1023

Servo servo0;
int servopin = 9;

void setup() {
  Serial.begin(9600);
  servo0.attach(servopin);
}

void loop() {
  // Reads light value from sensor on pin A0 every 1000ms (1s)
  // Prints raw input value and mapped (0 to 5) value  
  int sensor0 = analogRead(A0); 
  //not used
  int light_level0 = map(sensor0, SENSOR_MIN, SENSOR_MAX, 0, 5);

  int sensor1 = analogRead(A1); 
  //not used
  int light_level1 = map(sensor1, SENSOR_MIN, SENSOR_MAX, 0, 5);
  
  
  Serial.print("raw sensor0: ");
  Serial.println(sensor0);
  Serial.print("raw sensor1: ");
  Serial.println(sensor1);
  
  
  // add a "buffer zone" (+/- 10) so that sensor doesn't move constantly?
  if (sensor0 > (sensor1 + 5) && servo0.read() > 10)
  {
    //Serial.println("sensor0 brighter");
    // move towards sensor0
    Serial.println(servo0.read());
    servo0.write((servo0.read() - 5));
  }
  else if (sensor0 < (sensor1 - 5) && servo0.read() < 170)
  {
    //Serial.println("sensor1 brighter");
    // move towards sensor1
    Serial.println(servo0.read());
    servo0.write((servo0.read() + 5));
  }
  else
  {
    Serial.println(servo0.read());
    Serial.println("equal");
    // don't move
  }

  delay(2000);
}
