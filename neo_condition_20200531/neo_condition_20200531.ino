#include <Adafruit_NeoPixel.h>
#include <DHT.h>
//define NeoPixel Pin and Number of LEDs
#define PIN 11
#define NUM_LEDS 1
#define DHTPIN_internal 6
#define DHTTYPE DHT22

//create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
DHT dht_internal(DHTPIN_internal, DHTTYPE);
String data="RB";
float temp_internal;
void setColor(char C){
  if (C=='G'){
      strip.setPixelColor(0, 0, 255, 0);
      strip.show();
  } else if (C=='O'){
      strip.setPixelColor(0, 255, 150, 0);
      strip.show();
    } else if (C=='R'){
      strip.setPixelColor(0, 255, 0, 0);
      strip.show();
      }
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
  
  //Serial.println("Internal:");
  //Serial.print(F("\tTemperature: "));
  temp_internal = dht_internal.readTemperature();
  Serial.print(int(temp_internal));
  // set pixel to red, delay(1000) Green: strip.setPixelColor(0, 0, 255, 0); Orange: strip.setPixelColor(0, 255, 200, 0);
  setColor(data.charAt(0));
  delay(250);
  setBlink(data.charAt(1));
  delay(250);
  if (data.charAt(2)=='T'){
    temp_internal = dht_internal.readTemperature();
    
    Serial.print(int(temp_internal)); //int needed. otherwise it gets messed up
    }
  if(Serial.available()>0){
    data= Serial.readStringUntil('\n');

  }
  
}
