#include<DHT.h>
#define dhtPin 2
#define dhtType DHT11
DHT dht(dhtPin,dhtType);
int readSensor();
void setup() {
 // put your setup code here, to run once:
 Serial.begin(9600);
 dht.begin();
 pinMode(4,OUTPUT);

}
void loop() {
 // put your main code here, to run repeatedly:
 float t=dht.readTemperature();
 float h = dht.readHumidity();
 int r = analogRead(A3);
 int gas = analogRead(A2);
 if(isnan(t) || isnan(h) || isnan(r) || isnan(gas)){
 Serial.println("No data available");
 return;
 }
 if(r<400){
 digitalWrite(4,HIGH);
 }else{
 digitalWrite(4,LOW);
 }
 Serial.print(t);
 Serial.print(",");
 Serial.print(h);
 Serial.print("!");
 Serial.print(r);
 Serial.print("#");
 Serial.print(gas);

 Serial.println();
 delay(2000);
}
