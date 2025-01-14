#include <EEPROM.h>
#include <Servo.h>

#define SENSOR_MIN 0
#define SENSOR_MAX 1023

int sensor0 = analogRead(A0);
int sensor1 = analogRead(A1);  

// int pwradr = 0;

int centerswitch = 5;

Servo servo0;
int servopin = 9;

int resistor = 5;

int time;

void power_calc(void)
{
  // total energy generated since powered on 
  int volts = analogRead(A2);
  float power = (volts * volts) / resistor;
  time = millis();

  float energy = power * time;
  // EEPROM.put(pwradr, energy); 
}

int light_direction(void)
{
  if (sensor0 > (sensor1 + 5))
  {    
    // move towards sensor0
    return -1;
  }
  else if (sensor0 < (sensor1 - 5))
  {
    // move towards sensor1
    return 1;
  }
  else
  {
    // don't move
    return 0;
  }
}

void servo_move(void) {
  #if 0
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
  #endif
  int current = servo0.read();
  // move towards sensor while sensors are different 
  while (int direction = light_direction())
  {
    Serial.println(direction);
    Serial.println(servo0.read());
    int dr = direction * 4;
    
    if (current + dr > 10 && current + dr < 170)
    {
      servo0.write(dr);
    }
  }
}

void setup() {
  Serial.begin(9600);
  servo0.attach(servopin);
  pinMode(centerswitch, INPUT);
}

void loop() {
  // Reads light value from sensor on pin A0 every 1000ms (1s)
  
  //Serial.print("raw sensor0: ");
  //Serial.println(sensor0);
  //Serial.print("raw sensor1: ");
  //Serial.println(sensor1);
  
  // servo_move();
  /*float saved;
  EEPROM.get(pwradr, saved);
  Serial.println(saved);
  */
  //power_calc();
  // center motor and cancel movement if switch is on 
  if (digitalRead(centerswitch))
  {
    servo0.write(90);
  }
  else
  {
    servo_move();
  }
  delay(200);
}
