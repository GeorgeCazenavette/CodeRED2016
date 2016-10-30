#include <math.h> 

// Digital Pins
#define RED_PIN 5
#define GREEN_PIN 3
#define BLUE_PIN 6
#define FIRE_PIN 4
#define REMOTE_PIN 8
#define FIRE_MSG_PIN 7
//21 on RPi
#define ALARM_PIN 2
#define QUAKE_PIN 12
#define QUAKE_MSG_PIN 9
//20 on RPi
#define WATER_MSG_PIN 10
//16 on RPi
#define TEMP_MSG_PIN 11
//12 on RPi

// Analog Pins
#define TEMP_PIN 0
#define WATER_PIN 5

#define CRITICAL_TEMP 76

// Variables
int temp;
boolean fire;
boolean water;
String toPrint = "";
unsigned long lastUpdated = 0;
boolean quake;
boolean remote;
boolean danger = false;
boolean paused = false;

void setup() {
  // Initialize Pins for respective functions
  pinMode(FIRE_PIN, INPUT);
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(FIRE_MSG_PIN, OUTPUT);
  pinMode(QUAKE_MSG_PIN, OUTPUT);
  pinMode(WATER_MSG_PIN, OUTPUT);
  pinMode(TEMP_MSG_PIN, OUTPUT);
  pinMode(QUAKE_PIN, INPUT);
  pinMode(REMOTE_PIN, INPUT);

  // Begin serial communication with a BAUD rate of 9600
  Serial.begin(9600);

  // Write to LCD Screen
  Serial.write(22);
  Serial.write(12);
  Serial.write(17);
  Serial.write(128);
  Serial.print("Temp:  ");
  Serial.write(148);
  Serial.print("Fire:  ");
  Serial.write(168);
  Serial.print("Water: ");
  Serial.write(188);
  Serial.print("Quake: ");
}

void loop() 
{
  // Standby loop 
  StandbyCheck();

  // Clear toPrint string
  toPrint = "";

  // Read values from sensors
  temp = Thermister(analogRead(TEMP_PIN));
  fire = digitalRead(FIRE_PIN);
  water = analogRead(WATER_PIN) > 50;
  quake = digitalRead(QUAKE_PIN);
  remote = digitalRead(REMOTE_PIN);

  // If something has changed or 250 milliseconds have passed, then update LCD Screen
  if (millis() - lastUpdated >= 250 | millis() < lastUpdated | quake | fire | water | temp > CRITICAL_TEMP)
  {
    Serial.write(136);
    Serial.print("    ");
    Serial.write(136);
    Serial.print((temp > CRITICAL_TEMP) ? (String(temp) + "F CRITICAL") : (String(temp) + "F         "));
    Serial.write(156);
    Serial.print((fire) ? ("DETECTED") : ("Normal  "));
    Serial.write(176); 
    Serial.print((water) ? ("DETECTED") : ("Normal  "));
    Serial.write(196);
    Serial.print((quake) ? ("DETECTED") : ("Normal  "));
    lastUpdated = millis();
  }

  // Set of loops to raise Alarm() if needed
  if (fire)
  {
    digitalWrite(FIRE_MSG_PIN, HIGH);
    Alarm();
  }
  else
  {
    digitalWrite(FIRE_MSG_PIN, LOW);
  }
  if (quake)
  {
    digitalWrite(QUAKE_MSG_PIN, HIGH);
    Alarm();
  }
  else
  {
    digitalWrite(QUAKE_MSG_PIN, LOW);
  }
  if (water)
  {
    digitalWrite(WATER_MSG_PIN, HIGH);
    Alarm();
  }
  else
  {
    digitalWrite(WATER_MSG_PIN, LOW);
  }
  if (temp > CRITICAL_TEMP)
  {
    digitalWrite(TEMP_MSG_PIN, HIGH);
    Alarm();
  }
  else
  {
    digitalWrite(TEMP_MSG_PIN, LOW);
  }
  // ************************* NEED TO SETUP WATER MSG PIN ***************************************

  // Used to prevent flickering of RGB LED
  if (!danger)
  {
    analogWrite(RED_PIN, 0);
    analogWrite(BLUE_PIN, 0);
    analogWrite(GREEN_PIN, 255);
  }
  
  danger = false;
  
  
}

void StandbyCheck()
{
  // Set of loops to keep the arduino in a non-reactive state until released from standby
  if (digitalRead(REMOTE_PIN))
  {
    analogWrite(RED_PIN, 0);
    analogWrite(BLUE_PIN, 255);
    analogWrite(GREEN_PIN, 0);
    while (digitalRead(REMOTE_PIN))
    {
      ;
    }
    while (!digitalRead(REMOTE_PIN))
    {
      ;
    }
    while (digitalRead(REMOTE_PIN))
    {
      ;
    }
    analogWrite(RED_PIN, 0);
    analogWrite(BLUE_PIN, 0);
    analogWrite(GREEN_PIN, 255);
  }
}



// Function to translate rawADC data into Farenheit
double Thermister(int RawADC) { 
  double Temp; 
  Temp = log(((10240000/(1023-RawADC)) - 10000)); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp ); 
  Temp = (Temp - 273.15);            // Convert Kelvin to Celcius 
  return Temp + 46; 
} 

// Method to sound Alarm (piezo speaker) and set status LED to RED
void Alarm()
{
  danger = true;
  analogWrite(RED_PIN, 255);
  analogWrite(BLUE_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  for(float i=333;i<=1500;i=i+.5)
  {
    makeSound(ALARM_PIN,i);
  }
}

// Method to make piezo sound
void makeSound(int pin, float freq)
{
  digitalWrite(pin,HIGH);
  delayMicroseconds(int(1/freq/2*100000));
  digitalWrite(pin,LOW);
  delayMicroseconds(int(1/freq/2*100000));
}
