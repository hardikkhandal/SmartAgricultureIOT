#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>
#include <ThingESP.h>
#include <Servo.h>
Servo servo;
String URL = "";//enter the api key
ThingESP8266 thing("Username", "smartWeather", "name");
void sendData(float t, float h, float r, float gas);
String HandleResponse(String query)
{
  String data = Serial.readStringUntil('\n');
  Serial.println(data);
  int comma = data.indexOf(',');
  int exclam_for_rain = data.indexOf('!');
  int gashash = data.indexOf('#');
  float t = data.substring(0, comma).toFloat();
  float h = data.substring(comma + 1).toFloat();
  float r = data.substring(exclam_for_rain + 1).toFloat();
  float gas = data.substring(gashash + 1).toFloat();
  Serial.println("Temperature :");
  Serial.print(t);
  Serial.println("Humidity");
  Serial.print(h);
  Serial.println("Rain");
  Serial.print(h);
  Serial.println("Gas");
  Serial.print(h);
  String humid = "Humidity: " + String(h) + " % \n";
  String temp = "Temperature: " + String(t) + "°C \n";
  String Rain = "Rain: " + String(r) + " % \n";
  String Gas = "Gas: " + String(gas) + " \n";
  String ans;
  String rainSprink = "Sprinkler stopped for 5 seconds";
  String startsprink = "SPrinkler started again";
  if (query == "temperature") {
    return temp;
  }
  else if (query == "humidity") {
    return humid;
  }
  else if (query == "rain") {
    if (r <= 400) {
      ans = "Heavy Raining";
    } else if (r > 400 && r < 500) {
      ans = "Moderate Raining";
    }
    else {
      ans = "No Raining";
    }
    return ans;
  }
  else if (query == "gas") {
    return Gas;
  }
  else if (query == "stopsprink") {
    servo.write(0);
    delay(5000);
    return rainSprink;
  } else if (query == "startsprink") {
    servo.write(90);
    delay(1000);
    servo.write(0);
    delay(1000);
    return startsprink;
  }
  else return "Your query was invalid..";
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.disconnect();
  servo.attach(2); //D4
  servo.write(0);
  delay(2000);
  Serial.print("Start connection");
  WiFi.begin("Samsung_", "00000000");
  thing.SetWiFi("Samsung_", "00000000");
  thing.initDevice();
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(200);
    Serial.print("..");
  }
  Serial.println("Connected");
}
void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
    int comma = data.indexOf(',');
    int exclam_for_rain = data.indexOf('!');
    int gashash = data.indexOf('#');
    if (comma != -1 || exclam_for_rain != -1) {
      float t = data.substring(0, comma).toFloat();
      float h = data.substring(comma + 1).toFloat();
      float r = data.substring(exclam_for_rain + 1).toFloat();
      float gas = data.substring(gashash + 1).toFloat();
      Serial.print(t);
      Serial.println(h);
      Serial.println(r);
      Serial.println(gas);
      sendData(t, h, r, gas);
    }
  }
  servo.write(180);
  delay(1000);
  servo.write(90);
  delay(1000);
}
void sendData(float t, float h, float r, float gas) {
  WiFiClient client;
  HTTPClient http;
  String newUrl = URL + String(t) + "&field2=" + String(h) + "&field3=" + String(r) + "&field4=" +
                  String(gas);
  http.begin(client, newUrl);
  int responsecode = http.GET();
  String data = http.getString();
  Serial.println(data);
  http.end();
  thing.Handle();
}
