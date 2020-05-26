/*
  Demo code for Seer V2 kit feedback module

  Uses 2 x DHT22 temperature and humidity sensor
  Uses Adafruit LIS3DH accelerometer
  Uses Adafruit NeoPixel

*/

#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>

#define DHTPIN_internal 6     // Digital pin connected to the internal DHT sensor
#define DHTPIN_external 12     // Digital pin connected to the external DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define LED_PIN 11    // Digital pin connected to the NeoPixel
#define NUMPIXELS 1     // Number of pixels in the array

// Initialize DHT sensors
DHT dht_internal(DHTPIN_internal, DHTTYPE);
DHT dht_external(DHTPIN_external, DHTTYPE);

// Initialise accelerometer
Adafruit_LIS3DH accel = Adafruit_LIS3DH();
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_RGB + NEO_KHZ800);

float temp_internal;
float temp_external;
float humidity_internal;
float humidity_external;

void setup() {
  Serial.begin(9600);

  while (!Serial) delay(10);     // will pause until serial console opens

  if (! accel.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }

  accel.setRange(LIS3DH_RANGE_16_G);   // set accelerometer range 2, 4, 8 or 16 G!

  dht_internal.begin();
  dht_external.begin();

  pixels.begin(); // INITIALIZE NeoPixel strip object
  pixels.clear();
  pixels.setPixelColor(0, 100, 0, 0);
  pixels.show();

  delay(2000); // wait for DHT22 sensors - error otherwise
}

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity_internal = dht_internal.readHumidity();
  humidity_external = dht_external.readHumidity();

  // Read temperature as Celsius (the default)
  temp_internal = dht_internal.readTemperature();
  temp_external = dht_external.readTemperature();

  // Check if any reads failed and exit early (to try again).
  //  if (isnan(humidity_internal) || isnan(humidity_external) || isnan(temp_internal) || isnan(temp_external)) {
  //    Serial.println(F("Failed to read from DHT sensor!"));
  //    return;
  //  }

  // Read & print temperature & humidity
  Serial.println("External:");
  Serial.print(F("\tTemperature: "));
  Serial.print(temp_external);
  Serial.print(F("°C "));
  Serial.print(F("\tHumidity: "));
  Serial.print(humidity_external);
  Serial.println("%");

  Serial.println("Internal:");
  Serial.print(F("\tTemperature: "));
  Serial.print(temp_internal);
  Serial.print(F("°C "));
  Serial.print(F("\tHumidity: "));
  Serial.print(humidity_internal);
  Serial.println("%");

  accel.read();      // get X Y and Z data at once
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.println("Acceleration:");
  Serial.print("\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" m/s^2 ");
  Serial.print("\tY: "); Serial.print(event.acceleration.y);
  Serial.print(" m/s^2 ");
  Serial.print("\tZ: "); Serial.print(event.acceleration.z);
  Serial.println(" m/s^2 ");
  Serial.println();

  Serial.println();
  pixels.clear();
  pixels.setPixelColor(0, 100, 0, 0);
  pixels.show();



  // delay(500);
}
