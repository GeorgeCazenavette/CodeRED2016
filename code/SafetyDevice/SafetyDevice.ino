#include <math.h> 

// Digital Pins
#define FIRE_PIN 3
#define ALARM_PIN 5
// Analog Pins
#define TEMP_PIN 0

char degree = 167;
int temp;
boolean fire;
String toPrint = "";

void setup() {
  // put your setup code here, to run once:
  pinMode(3, INPUT);
  pinMode(ALARM_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  toPrint = "";
  temp = analogRead(TEMP_PIN);
  fire = digitalRead(FIRE_PIN);
  toPrint += "Temperature: ";
  toPrint += temp;
  toPrint += "\tFire: ";
  toPrint += (fire) ? ("FIRE") : ("NO Fire :(");
  Serial.println(toPrint);
  if (fire)
  {
    Alarm();
  }
}





double Thermister(int RawADC) { 
  double Temp; 
  Temp = log(((10240000/RawADC) - 10000)); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp ); 
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius 
  return Temp; 
} 

void Alarm()
{
  for(int i = 0; i == 10000000; i++)
  {
    digitalWrite(ALARM_PIN, HIGH);
    delayMicroseconds(200);
    digitalWrite(ALARM_PIN, LOW);
    delayMicroseconds(200);
  }
}

void makeSound(int pin, float freq)
{
  digitalWrite(pin,HIGH);
  delayMicroseconds(int(1/freq/2*100000));
  digitalWrite(pin,LOW);
  delayMicroseconds(int(1/freq/2*100000));
}

void makeSound(int pin, float freq, float dur)
{
  for(float t=1; t<=dur/(1/freq);t++)
  {
    digitalWrite(pin,HIGH);
    delayMicroseconds(int(1/freq/2*100000));
    digitalWrite(pin,LOW);
    delayMicroseconds(int(1/freq/2*100000));
  }
}
