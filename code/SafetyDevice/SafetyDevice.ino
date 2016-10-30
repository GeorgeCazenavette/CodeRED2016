#include <math.h> 

// Digital Pins
#define FIRE_PIN 3
#define ALARM_PIN 5
// Analog Pins
#define TEMP_PIN 0
#define WATER_PIN 1

char degree = 167;
int temp;
boolean fire;
int water;
String toPrint = "";
unsigned long lastUpdated = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(3, INPUT);
  pinMode(ALARM_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.write(22);
  Serial.write(12);
  Serial.write(17);
  Serial.write(128);
  Serial.print("Temp: ");
  Serial.write(148);
  Serial.print("Status: ");
  Serial.write(168);
  Serial.print("Water: ");
}

void loop() {
  // put your main code here, to run repeatedly:
  toPrint = "";
  temp = analogRead(TEMP_PIN);
  fire = digitalRead(FIRE_PIN);
  water = analogRead(WATER_PIN);
  /*toPrint += "Temperature: ";
  toPrint += temp;
  toPrint += "\tFire: ";
  toPrint += (fire) ? ("FIRE") : ("NO Fire :(");*/
  if (millis() - lastUpdated >= 250 | millis() < lastUpdated | fire)
  {
    Serial.write(134);
    Serial.print("    ");
    Serial.write(134);
    Serial.print(temp);
    Serial.write(156);
    Serial.print((fire) ? ("FIRE       ") : ("NO Fire :("));
    Serial.write(175);
    Serial.print("    ");
    Serial.write(175);
    Serial.print(water);
    lastUpdated = millis();
  }
    //Serial.println(toPrint);
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
  for(float i=333;i<=1500;i=i+.5)
    {
      makeSound(ALARM_PIN,i);
    }
}

void makeSound(int pin, float freq)
{
  digitalWrite(pin,HIGH);
  delayMicroseconds(int(1/freq/2*100000));
  digitalWrite(pin,LOW);
  delayMicroseconds(int(1/freq/2*100000));
}


