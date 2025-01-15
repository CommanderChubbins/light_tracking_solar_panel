  #include <EEPROM.h>
  #include <Servo.h>

  #define SENSOR_MIN 0
  #define SENSOR_MAX 1023
  #define RANGES 3
  //int avgsensor0 = analogRead(A0);
  //int avgsensor1 = analogRead(A1);  

  // int pwradr = 0;

  // max, angle 
  int difftoangle[RANGES][2] = {
    {10, 1},
    {30, 2},
    {50, 4}
  }

  int centerswitch = 7;

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
    // average sensors 5 times 
    // add input sample amount & delay
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

    int diff = abs(avgsensor0 - avgsensor1);

    // search table for angle based on difference 
    for (int i = 0; i < RANGES; ++i)
    {
      // greater than maximum for an angle 
      if (diff > difftoangle[i][0])
      {
        // wrong pair
      }
      else
      {
        // right pair
      }
    }
    #if 0
    // change angle based on difference in sensor readings  
    if (avgsensor0 > (avgsensor1 + 10))
    {    
      // move towards avgsensor0
      //Serial.println(avgsensor0);
      //Serial.println(avgsensor1 + 10);
      return -1;
    }
    else if (avgsensor0 < (avgsensor1 - 10))
    {
      // move towards avgsensor1
      //Serial.println(avgsensor1);
      return 1;
    }
    else
    {
      // don't move
      //Serial.print(avgsensor0);
      //Serial.println(avgsensor1);
      return 0;
    }
    #endif

  }

  void servo_move(int direction) {
    // move motor towards sensors
    int current = servo0.read();
    int dr = current + (direction * 4);

    if (dr > 10 && dr < 170)
    {
      servo0.write(dr);
    }
  }

  void setup() {
    Serial.begin(9600);
    servo0.attach(servopin);
    pinMode(centerswitch, INPUT);
  }

  void loop() {
    /*float saved;
    EEPROM.get(pwradr, saved);
    Serial.println(saved);
    */

    int count = Serial.available();
    if (count)
    {
      Serial.print("recieved: ");
      Serial.println(Serial.readString());
    }

    // center motor and cancel movement if switch is on 
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
