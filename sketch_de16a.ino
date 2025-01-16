//#include <EEPROM.h>
#include <Servo.h>

#define SENSOR_MIN 0
#define SENSOR_MAX 1023
#define RANGE_COUNT 4
//int avgsensor0 = analogRead(A0);
//int avgsensor1 = analogRead(A1);  

// int pwradr = 0;

// max, angle 
int difftoangle[RANGE_COUNT][2] = {
  {10, 0},
  {50, 1},
  {80, 2},
  {1024, 5}
};

int centeringpin = 7;

Servo servo0;
int servopin = 3;

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


// Averages sensor readings every 200ms, 5 times
// This increases measurement accuracy and reduces margin of error
// Returns the difference between the sensors 
int sensor_avg(void)
{
  int avgsensor0 = 0;
  int avgsensor1 = 0;

  for (int i = 0; i < 5; ++i)
  {
    avgsensor0 += analogRead(A0);
    avgsensor1 += analogRead(A1);
    delay(200);
  }
  avgsensor0 /= 5;
  avgsensor1 /= 5;

  return avgsensor1 - avgsensor0;
}

int get_angle(void)
{
  int diff = sensor_avg();
  Serial.print("difference: ");
  Serial.println(diff);
  // search table for angle based on difference 
  for (int i = 0; i < RANGE_COUNT; ++i)
  {
    // greater than maximum for an angle 
    if (abs(diff) > difftoangle[i][0])
    {
      // wrong pair
      continue;
    }
    else
    {
      // right pair
      int angle = difftoangle[i][1];
      Serial.print("angle: ");
      Serial.println((diff < 0 ? angle : -angle));
      return (diff < 0 ? angle : -angle);
    }
  }
}

void servo_move(int angle) {
  // move motor towards sensors
  int current = servo0.read();
  int dr = current + angle;
  int direction = abs(angle)/angle;
  // Serial.println(current);
  if (dr > 15 && dr < 165)
  {
    for (int i = 0; i < abs(angle); ++i)
    {
      servo0.write(current + direction);
      current = servo0.read();
      delay(50);
    }
  }
  Serial.print("current angle: ");
  Serial.println(servo0.read());
  return;
  /*if (dr > 20 && dr < 160)
  {
    servo0.write(dr);
  }*/
}

void setup() {
  Serial.begin(9600);
  servo0.attach(servopin);
  pinMode(centeringpin, INPUT);
}

void loop() {
  /*float saved;
  EEPROM.get(pwradr, saved);
  Serial.println(saved);
  */

  // Serial read 
  int count = Serial.available();
  if (count)
  {
    Serial.print("recieved: ");
    Serial.println(Serial.readString());
  }

  // center motor and cancel movement if switch is on 
  if (digitalRead(centeringpin))
  {
      servo0.write(90);
  }
  else if (int angle = get_angle())
  {
    servo_move(angle);
  }

  // to add:
    // figure out if light direction is "found" or stable 
    // then, if a sudden change occurs, pause for n time before checking and moving   again

  delay(500);
}
