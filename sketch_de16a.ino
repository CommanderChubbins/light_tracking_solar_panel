#include <EEPROM.h>
#include <Servo.h>

#define SENSOR_MIN 0
#define SENSOR_MAX 1023

int sensor0 = analogRead(A0); 
int sensor1 = analogRead(A1);   

int pwradr = 0;

Servo servo0;
int servopin = 9;

int vpin = A2;
int resistor = 5;

void power_calc(void)
{
  int volts = 5; //analogRead(vpin);
  float power = (volts * volts) / resistor;
  int time = 3;

  float wh = power * time;
  EEPROM.put(pwradr, wh); 
}

int light_direction(void)
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

void servo_move(void) {
  int current = servo0.read();
  int dr = 1;
  while (int direction = light_direction())
  {
    
    servo0.write(current + dr);
    dr++;
    current = servo0.read();
  }
  //servo0.write(current);
  //servo0.write(0);
  
}

void setup() {
  Serial.begin(9600);
  servo0.attach(servopin);
}

void loop() {
  // Reads light value from sensor on pin A0 every 1000ms (1s)
  
  //Serial.print("raw sensor0: ");
  //Serial.println(sensor0);
  //Serial.print("raw sensor1: ");
  //Serial.println(sensor1);
  
  // servo_move();
  float saved;
  EEPROM.get(pwradr, saved);
  Serial.println(saved);
  //power_calc();

  delay(20);
}
