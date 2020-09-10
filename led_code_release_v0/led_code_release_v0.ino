
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

// baudrate to use in the usb serial connection
const long baudRate = 9600;
// the last time (in millis) the LED's state was flipped when blinking is enabled
unsigned long led_blink_previousMillis = 0;
// how often (in millis) to flip the LED's state when blinking is enabled
const long led_blink_interval = 500;
// the last time (in millis) a command was received over serial from the Hub
unsigned long serial_data_time=0;
// the last time (in millis) the WDT was automatically fed
unsigned long wdt_last_auto_feed_time=0;
// max times the WDT is feed/reset automatically when no comman is received from the hub before we just let the WDT trigger itself
const int wdt_max_consecutive_feeds = 40;
// how long to wait between each automatic WDT reset 
const long wdt_wait = 500;
// maximum measured acceleration value so far (resets when hub reads it out)
float accl_max = 0;
int wdt_n_auto_consecutive_feeds = 0;
// did the accelerometer start?
int acc_start = 0;
// currently set RGB values for the LED
int ledR = 0;
int ledG = 0;
int ledB = 0;
// is the LED currently OFF or ON
int ledState = LOW;
// is the LED blinking or fixed?
bool ledBlinking = false;
//create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
// internal temperature/humidity sensor
DHT dht_internal(DHTPIN_internal, DHTTYPE);
// external temperature/humidity sensor
DHT dht_external(DHTPIN_external, DHTTYPE);
// Initialise accelerometer
Adafruit_LIS3DH accel = Adafruit_LIS3DH();


void setup() {
    wdt_enable(9);
    //Serial.end();
    //delay(1000);
    // start the strip and blank it out
    String data = "000000100SZ";
    Serial.begin(baudRate);
    
    while (!Serial); // wait for serial to be ready (for USB CDC based serial connections)

    if (!accel.begin(0x18)) { // change this to 0x19 for alternative i2c address
        //Serial.println("Couldnt start");
        acc_start = 0;
        while (1)
            yield();
    } else {
        acc_start = 1;
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

/**
 * Configure the RGB color to use when the LED is ON
 */
void setColor(String data) {
    ledR = data.substring(0, 3).toInt();
    ledG = data.substring(3, 6).toInt();
    ledB = data.substring(6, 9).toInt();
}

/**
 * Switch on the LED with the configured RGB values as its color
 */
void lightLED() {
    strip.setPixelColor(0, ledR, ledG, ledB);
    strip.show();
    ledState = HIGH;
}

/**
 * Switch OFF the LED
 * -- without clearing the saved RGB values (we'll use those when we switch the LED back on again)
 */
void ledOff() {
    strip.setPixelColor(0, 0, 0, 0);
    strip.show()
    ledState = LOW;
}


/**
 * Helper function used by handle_received_command()
 * It sets the LED mode and updates the LED variables and state accordingly.
 */
void setLEDMode(String data, bool blinking_enabled) {
    // update the desired RGB color
    setColor(data);
    // set blinking mode
    ledBlinking = blinking_enabled;
    // if the LED is OFF, and blinking is DISABLE, then switch on the LED
    if(!ledBlinking && ledState == LOW) {
        lightLED();
    }
    // if the LED is already ON, then update its RGB values immediately (regardless of mode)
    else if(ledState == HIGH) {
        lightLED();
    }
}


void handle_received_command(String data) {
    // Set LED to Fixed Color
    if (data.charAt(9) == 'S') {
        setLEDMode(data, false);
    }
    // Enable LED Blink
    else if(data.charAt(9) == 'B') {
        setLEDMode(data, true);
    }
    // Internal Tempratue
    if (data.charAt(10) == 'T') {
        float temp_internal = dht_internal.readTemperature();
        Serial.println(temp_internal);
    }
    // Internal Humidity
    else if (data.charAt(10) == 'H') {
        float hum_int = dht_internal.readHumidity();
        Serial.println(hum_int);
    }
    // External Tempratue
    else if (data.charAt(10) == 'E') {
        float temp_ext = dht_external.readTemperature();
        Serial.println(temp_ext);
    }
    // Enternal Humidity
    else if (data.charAt(10) == 'I') {
        float hum_ext = dht_external.readHumidity();
        Serial.println(hum_ext);
    }
    // Acceleration
    else if (data.charAt(10) == 'A') {
        if (!acc_start) {
            Serial.println("Couldnt start");
        }
        else {
            Serial.println(accl_max);
            accl_max = 0;
        }
    }
}

void update_accelerometer() {
    if (acc_start == 1) {
        accel.read();
        sensors_event_t event;
        accel.getEvent(&event);
        float accl = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y) + sq(event.acceleration.z));
        if (accl > accl_max) {
            accl_max = accl;
        }
    }
}

/**
 * Check and feed the Watchdog Timer.
 * This function will feed the Watchdog every X milli-seconds (as set by the `wdt_wait` variable).
 * This function will feed the Watchdog at most N consecutive times (as set by the `wdt_max_consecutive_feeds` variable).
 * If too much time has passed since the last command received from the Hub,
 * then don't feed the Watchdog and let it trigger.
 */
void check_wdt() {
    unsigned long currentMillis = millis();
	if ((currentMillis-wdt_last_auto_feed_time)>=wdt_wait) {
		wdt_n_auto_consecutive_feeds++;
		wdt_last_auto_feed_time = currentMillis;
		if (wdt_n_auto_consecutive_feeds < wdt_max_consecutive_feeds){
			wdt_reset();
		}
    }
}

/**
 * Called when a command is received over Serial from the Hub.
 * Feeds the WDT and also resets the counter for Number of Consecutive Watchdog feeds that is used to determine
 * when to stop automatically feeding the watchdog and let it reset if no command is received.
 */
void feed_wdt() {
    wdt_n_auto_consecutive_feeds = 0;
    wdt_reset();
}

/**
 * Animate the LED's blinking, if blinking has been enabled
 */
void update_led_blink_state() {
    // set pixel to red, delay(1000) Green: strip.setPixelColor(0, 0, 255, 0); Orange: strip.setPixelColor(0, 255, 200, 0);
    unsigned long currentMillis = millis();
    // if blinking is enabled, then toggle between LED ON and LED OFF periodically
    if (ledBlinking && (currentMillis - led_blink_previousMillis >= led_blink_interval)) {
        // save the last time you blinked the LED
        //Serial.println("IN");
        led_blink_previousMillis = currentMillis;
        // LED is ON --> turn it OFF
        if (ledState == HIGH) {
            ledOff();
        }
        // LED is OFF --> turn it ON
        else {
            lightLED();
        }
    }
    

}

/**
 * Read a new command from the serial port (if available)
 * and parse that command and execute the relevant logic
 */
void read_and_parse_new_command() {
    if (Serial && Serial.available() > 0)
    {
        String str = Serial.readStringUntil('\n');
        if (str.length() == 11){
            feed_wdt();
            serial_data_time = currentMillis;
            handle_received_command(str);
        }
    }
}

/**
 * Arduino Loop
 */
void loop() {
    check_wdt();
    update_accelerometer();
    read_and_parse_new_command();
    update_led_blink_state();
}

