/*
 * Integrated Digital Clock: DS3231 RTC + SSD1306 OLED
 * Hardware Connections:
 * Arduino A4 (SDA) -> RTC SDA & OLED SDA
 * Arduino A5 (SCL) -> RTC SCL & OLED SCL
 * Arduino 5V       -> VCC (Both)
 * Arduino GND      -> GND (Both)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

// OLED display settings
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 // Changed to 64 for better visibility, adjust to 32 if needed
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 

// Initialize objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);

  // 1. Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Initializing...");
  display.display();

  // 2. Initialize RTC
  if (!rtc.begin()) {
    display.println("Couldn't find RTC");
    display.display();
    while (1);
  }

  // 3. Set time if power was lost
  if (rtc.lostPower()) {
    // Sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  delay(1000);
}

void loop() {
  DateTime now = rtc.now();

  display.clearDisplay(); // Always clear the buffer before drawing

  // --- Display Date ---
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(now.day(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.year(), DEC);

  // --- Display Temperature ---
  display.setCursor(85, 0);
  display.print(rtc.getTemperature(), 1); // 1 decimal place
  display.print("C");

  // --- Display Time (Large Text) ---
  display.setTextSize(2); // Larger scale for the actual clock
  display.setCursor(15, 25);
  
  // Format hours (Add leading zero if < 10)
  if (now.hour() < 10) display.print('0');
  display.print(now.hour(), DEC);
  display.print(':');
  
  // Format minutes (Add leading zero if < 10)
  if (now.minute() < 10) display.print('0');
  display.print(now.minute(), DEC);
  display.print(':');
  
  // Format seconds
  if (now.second() < 10) display.print('0');
  display.print(now.second(), DEC);

  // --- Draw a simple border or line ---
  display.drawLine(0, 12, 128, 12, SSD1306_WHITE);

  // Push the buffer to the physical screen
  display.display();

  delay(1000); // Update every second
}