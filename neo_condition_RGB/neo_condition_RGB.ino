#include <Adafruit_NeoPixel.h>
#include <DHT.h>
//define NeoPixel Pin and Number of LEDs
#define PIN 11
#define NUM_LEDS 1
#define DHTPIN_internal 6
#define DHTTYPE DHT22
float temp_internal;
float humid;
//create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
DHT dht_internal(DHTPIN_internal, DHTTYPE);
String data="000255000BT";

void setColor(){

  int R=data.substring(0,3).toInt();
  int G=data.substring(3,6).toInt();
  int B=data.substring(6,9).toInt();
  strip.setPixelColor(0, R, G, B);

  strip.show();
}
 void setBlink(char B){
  if (B=='B'){
     strip.setPixelColor(0, 0, 0, 0);
     strip.show();
    }
  }

void setup() {
  // start the strip and blank it out
  strip.begin();
  strip.show();
  Serial.begin(9600);
  Serial.setTimeout(1);
  dht_internal.begin();
}
void loop() {
  //Serial.println();
  //Serial.println("Internal:");
  //Serial.print(F("\tTemperature: "));

  //Serial.print([charValT,charValH]);
  //Serial.print(charValH);
  //Serial.print(int(humid_internal)); //int needed. otherwise it gets messed up
  
  // set pixel to red, delay(1000) Green: strip.setPixelColor(0, 0, 255, 0); Orange: strip.setPixelColor(0, 255, 200, 0);
  setColor();
  delay(250);
  setBlink(data.charAt(9));
  delay(250);
  if (data.charAt(10)=='T'){
      temp_internal = dht_internal.readTemperature();
    Serial.println(temp_internal); //int needed. otherwise it gets messed up
    data.setCharAt(10,'Z');
    }
  if (data.charAt(10)=='H'){
    humid = dht_internal.readHumidity();
    Serial.println(humid); //int needed. otherwise it gets messed up
    data.setCharAt(10,'Z');
    }
if(Serial.available()>0){
      String str = Serial.readStringUntil('\n');
      if (str.length == 11) {
          data = str;
      }
  }
