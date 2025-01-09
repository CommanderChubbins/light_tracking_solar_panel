#include <Servo.h>

#define SENSOR_MIN 0
#define SENSOR_MAX 1023

Servo servo0;
int servopin = 9;

int light_direction(int sensor0, int sensor1)
{
  if (sensor0 > (sensor1 + 5))
  {    
    // move towards sensor0
    return sensor0;
  }
  else if (sensor0 < (sensor1 - 5))
  {
    // move towards sensor1
    return sensor1;
  }
  else
  {
    // don't move
    return 0;
  }
}

void servo_move(bool start) {
  //int current = servo0.read();
  //servo0.write(current);
  //servo0.write(0);

}

void setup() {
  Serial.begin(9600);
  // Serial1.begin(9600);
  servo0.attach(servopin);
}

void loop() {
  // Reads light value from sensor on pin A0 every 1000ms (1s)
  // Prints raw input value and mapped (0 to 5) value  
  int sensor0 = analogRead(A0); 

  int sensor1 = analogRead(A1);   
  
  Serial.print("raw sensor0: ");
  Serial.println(sensor0);
  Serial.print("raw sensor1: ");
  Serial.println(sensor1);
  


  delay(20);
}
