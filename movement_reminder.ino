
//import libraries
#include "U8glib.h"
#include <RtcDS3231.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>


//definitions 
#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN    10
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 24
#define BUTTON_PIN 2
#define theaterChase
#define rainbow
#define theaterChaseRainbow


U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);
//Declare our NeoPixel strip object


// Create variables
byte hours;
byte minutes;
byte seconds;
String timeShow;
float temp = 0;
String getMove;




// Creating new RTC modul
RtcDS3231<TwoWire> rtcModule(Wire);
Servo myservo;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);


// Set up (loops once) and variables that is needed later
void setup() {
  
  //comment away?
  pinMode(BUTTON_PIN, INPUT);
  // 
  
  //Serial.begin(9600);

  // Begin serial
  Serial.begin(9600);
  rtcModule.Begin();
  myservo.attach(9);

  // Start Neopixel ring
  strip.begin();
  strip.setBrightness(50);
  strip.show();

  // Update RTC module time to compilation time
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  rtcModule.SetDateTime(compiled);

  Serial.begin(9600);
}


// writes time and temprature and makes the rtc module get the time and temperaturer
void rtcUpdate(void) {
  RtcDateTime now = rtcModule.GetDateTime();
  RtcTemperature rtcTemp = rtcModule.GetTemperature();

  temp = rtcTemp.AsFloatDegC();
  hours = now.Hour();
  minutes = now.Minute();
  seconds = now.Second();

  timeShow = (String(hours) + ": " + String(minutes) + ": " + String(seconds) + ", " + String(temp) + char(176) + "C");
  getMove = "Get up and move ;)";
}

void loop() {
  //not needed? -->
  int buttonPressed = digitalRead(BUTTON_PIN);
  // <---

  //Update time from clock modlue
  rtcUpdate();
  UpdateOled();
  UpdateWakeup();

  // Fills along the length of the strip in various colors every time the minutes are equal 
  //to zero (every new hour) and less than 30 seconds (aka it runs for 30sec when its a new hour)
  //

  if (minutes == 0 && seconds <= 30)
  {
    colorWipe(strip.Color(255,   0,   0), 50); // Red
    colorWipe(strip.Color(  0, 255,   0), 50); // Green
    colorWipe(strip.Color(  0,   0, 255), 50); // Blue

    // // Do a theater marquee effect in various colors... (not  using...)
    // theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
    // theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
    // theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

    rainbow(10);             // Flowing rainbow cycle along the whole strip
    // theaterChaseRainbow(50);  // Rainbow-enhanced theaterChase variant
  }

}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(10);                           //  Pause for a moment
  }
}



// updates oled screen so it shows time with a font

void UpdateOled() {
    //if time is between 0 min and 30 to 35 seconds it will show getMove that has the string 
    //"Get up and Move". If it isn't 0 min and between 30 to 35 sec it will show what time it is as well
    //as the temprature
  if (minutes == 0 && seconds <= 30 && seconds <= 35) {
    oled.firstPage();
    do {
      oled.setFont(u8g_font_helvB10);
      oled.drawStr(1, 40, String(getMove).c_str());
    } while (oled.nextPage());
  }
  else {
    oled.firstPage();
    do {
      oled.setFont(u8g_font_helvB10);
      oled.drawStr(1, 31, String(timeShow).c_str());
    } while (oled.nextPage());
  }
}




// Make circle blink every hour to remind
void UpdateWakeup() {
  for (int i = 0; i <= minutes; i++)
  {
    strip.setPixelColor (map(i, 0, 60, 1, 24), strip.Color(0 , 0, 255));
  }
  if (minutes == 0 && seconds >= 30 && seconds <= 35) {
    for (int x = 0; x <= 24; x++) {
    strip.setPixelColor (x, strip.Color(0 , 0, 0));
    delay(100);
    }
  }
  delay(50);
  strip.show();
  Serial.print("Update Wakeup");
}