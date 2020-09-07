
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

unsigned long previousMillis = 0;
//unsigned long serial_data_time = 0;
int ledState = LOW;
const long interval = 500;
const long serial_wait = 10000;
unsigned long serial_data_time=0;
unsigned long last_time=0;
const long wdt_wait = 500;
float temp_internal;
float temp_ext;
float humid;
float accl;
float accl_max = 0;
String data = "000000100ST";
int count = 0;
int acc_start = 1;
int i=0;
//create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
DHT dht_internal(DHTPIN_internal, DHTTYPE);
DHT dht_external(DHTPIN_external, DHTTYPE);

// Initialise accelerometer
Adafruit_LIS3DH accel = Adafruit_LIS3DH();

void setColor()
{

    int R = data.substring(0, 3).toInt();
    int G = data.substring(3, 6).toInt();
    int B = data.substring(6, 9).toInt();
    strip.setPixelColor(0, R, G, B);

    strip.show();
}
void setBlink(char B)
{
    if (B == 'B')
    {
        strip.setPixelColor(0, 0, 0, 0);
        strip.show();
    }
}

void setup()
{
    wdt_enable(9);
    //Serial.end();
    //delay(1000);
    // start the strip and blank it out
    String data = "000000100ST";
    Serial.begin(9600);
    while (!Serial)
        delay(10);
    if (!accel.begin(0x18))
    { // change this to 0x19 for alternative i2c address
        //Serial.println("Couldnt start");
        acc_start = 0;
        while (1)
            yield();
    }

    accel.setRange(LIS3DH_RANGE_16_G);

    dht_internal.begin();
    dht_external.begin();

    strip.begin();
    strip.clear();
    strip.show();

    //delay(10);

    //Serial.setTimeout(1);
}
void loop()
{

    // set pixel to red, delay(1000) Green: strip.setPixelColor(0, 0, 255, 0); Orange: strip.setPixelColor(0, 255, 200, 0);

    if (acc_start == 1)
    {
        accel.read();
        sensors_event_t event;
        accel.getEvent(&event);
        accl = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y) + sq(event.acceleration.z));
        if (accl > accl_max)
        {
            accl_max = accl;
        }
    }
    unsigned long currentMillis = millis();
	if ((currentMillis-last_time)>=wdt_wait) {
		i++;
		last_time = currentMillis;
		if (i<40){
			wdt_reset();
		}
    }
	/*
    if ((currentMillis - serial_data_time) >= serial_wait)
    {
        data = "000000100SZ";
    }
	*/
    if ((currentMillis - previousMillis >= interval) && (data.charAt(9) == 'B'))
    {
        // save the last time you blinked the LED
        //Serial.println("IN");
        previousMillis = currentMillis;
        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
        {
            strip.setPixelColor(0, 0, 0, 0);
            strip.show();
            ledState = HIGH;
        }
        else
        {
            setColor();
            ledState = LOW;
        }
    }
    if (data.charAt(9) == 'S')
    {
        setColor();
    }
    if (data.charAt(10) == 'T')
    {
        temp_internal = dht_internal.readTemperature();
        Serial.println(temp_internal);
        data.setCharAt(10, 'Z');
    }
    else if (data.charAt(10) == 'H')
    {
        humid = dht_internal.readHumidity();
        Serial.println(humid);
        data.setCharAt(10, 'Z');
    }
    else if (data.charAt(10) == 'E')
    {
        temp_ext = dht_external.readTemperature();
        Serial.println(temp_ext);
        data.setCharAt(10, 'Z');
    }
    else if (data.charAt(10) == 'I')
    {
        temp_ext = dht_external.readHumidity();
        Serial.println(temp_ext);
        data.setCharAt(10, 'Z');
    }
    else if (data.charAt(10) == 'A')
    {   if (!acc_start){
          Serial.println("Couldnt start");
        } else{
        Serial.println(accl_max);
        accl_max = 0;
        data.setCharAt(10, 'Z');
        }
    }

    if (Serial.available() > 0)
    {
        String str = Serial.readStringUntil('\n');
        if (str.length() == 11)
        {   i=0;
            data = str;
            wdt_reset();
            serial_data_time = currentMillis;
        }
    }
}
