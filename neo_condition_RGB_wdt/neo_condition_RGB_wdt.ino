
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <avr/wdt.h>

//define NeoPixel Pin and Number of LEDs


#define PIN 11
#define NUM_LEDS 1

#define DHTPIN_internal 6
#define DHTPIN_external 12    
#define DHTTYPE DHT22
float temp_internal;
float humid;
String data="000000255ST";
//create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
DHT dht_internal(DHTPIN_internal, DHTTYPE);
DHT dht_external(DHTPIN_external, DHTTYPE);

// Initialise accelerometer
Adafruit_LIS3DH accel = Adafruit_LIS3DH();



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
  Serial.end();
  delay(10000); 
  // start the strip and blank it out
  String data="000000255ST";
  Serial.begin(9600);
  while (!Serial) delay(10);  
  if (! accel.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }

  accel.setRange(LIS3DH_RANGE_16_G);  

  dht_internal.begin();
  dht_external.begin();
  
  strip.begin();
  strip.clear();
  strip.show();

  delay(100); 
  wdt_enable(9);
  //Serial.setTimeout(1);

}
void loop() {

  //
  // set pixel to red, delay(1000) Green: strip.setPixelColor(0, 0, 255, 0); Orange: strip.setPixelColor(0, 255, 200, 0);
  setColor();
  delay(250);
  setBlink(data.charAt(9));
  delay(250);
  /*
  if (data.charAt(10)=='T'){
      temp_internal = dht_internal.readTemperature();
    Serial.println(temp_internal); 
    data.setCharAt(10,'Z');
    }
  if (data.charAt(10)=='H'){
    humid = dht_internal.readHumidity();
    Serial.println(humid); 
    data.setCharAt(10,'Z');
    }
   */ 
if(Serial.available()>0){
      String str = Serial.readStringUntil('\n');
      if (str.length() == 11) {
          data = str;
          wdt_reset();
      }
      }
  }
