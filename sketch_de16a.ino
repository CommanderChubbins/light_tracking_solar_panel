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
  // average sensors ~5 times 
  sensor0 = analogRead(A0);
  sensor1 = analogRead(A1);
  
  // change angle based on difference in sensor readings  
  if (sensor0 > (sensor1 + 10))
  {    
    // move towards sensor0
    //Serial.println(sensor0);
    //Serial.println(sensor1 + 10);
    return -1;
  }
  else if (sensor0 < (sensor1 - 10))
  {
    // move towards sensor1
    //Serial.println(sensor1);
    return 1;
  }
  else
  {
    // don't move
    //Serial.print(sensor0);
    //Serial.println(sensor1);
    return 0;
  }
}

void servo_move(int direction) {
  // move towards sensor while sensors are different 
  int current = servo0.read();
  
  //Serial.println(servo0.read());
  int dr = current + (direction * 4);

  if (dr > 10 && dr < 170)
  {
    servo0.write(dr);
  }
  
  //while (int direction = light_direction())
  
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
  int count = Serial.available();
  if (count)
  {
    Serial.print("recieved: ");
    Serial.println(Serial.readString());
  }

  
  if (digitalRead(centerswitch))
  {
    servo0.write(90);
  }
  // figure out if light direction is "found" or stable 
  // then, if a sudden change occurs, pause for n time before checking and moving   again
  else if (int direction = light_direction())
  {
    servo_move(direction);
  }
  delay(500);
}
