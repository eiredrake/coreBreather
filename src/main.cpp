#include <Arduino.h>
// #include "avr8-stub.h"
// #include "app_api.h"
#include "Adafruit_DotStar.h"
// #include "SPI.h"
#include "Regexp.h"

// HOW TO SET THIS SHIT UP: https://www.youtube.com/watch?v=7wx27FcluMg&ab_channel=J%27se-shack

#define NUM_PIXELS 50
#define DATA_PIN 4
#define CLOCK_PIN 5
Adafruit_DotStar strip(NUM_PIXELS, DATA_PIN, CLOCK_PIN, DOTSTAR_BRG);

uint32_t color = 0xFF00FF;
uint8_t min_brightness = 0;
uint8_t max_brightness = 50;
uint32_t breath_speed = 5;
uint32_t hold_breath_time = 50;
uint8_t mode = 0;

#define BREATHE_MODE 1

void NullMode(uint8_t sleep_ms);
void BreatheMode(uint32_t color, uint8_t min_brightness, uint8_t max_brightness, uint32_t breathe_speed, uint32_t hold_breath_ms);

void setup() {
  // debug_init();

  Serial.begin(9600);
  Serial.println("min_brightness: " + String(min_brightness));
  Serial.println("max_brightness: " + String(max_brightness));
  Serial.println("breath_speed: " + String(breath_speed) + " ms.");
  Serial.println("hold_breath_time: " + String(hold_breath_time) + " ms.");
  Serial.println("color: " + String(color));

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  for(int index = 0; index < NUM_PIXELS; index ++ )
  {
    strip.setPixelColor(index, color); // 'On' pixel at head  
  }  

  Serial.println("Core processor is listening....");

  mode = 1;
}

void loop() {
  if(Serial.available())
  {
    String input = Serial.readString();
    if (input.length() <= 200)
    {
      char* buffer = const_cast<char*>(input.c_str());
      const uint32_t length = input.length();

      MatchState regex;
      regex.Target(buffer, length);
      char result = regex.Match("^([a-z]+): (\\d*)");
      if (result == REGEXP_MATCHED)
      {
          Serial.println("Captures: " + String(regex.level));
          for(int index = 0; index < regex.level; index++)
          {
              String capture = regex.GetCapture(buffer, index);
              Serial.println("Capture: " + capture);
          }

          String command = regex.GetCapture(buffer, 0);
          String argument = regex.GetCapture(buffer, 1);

          Serial.println("Command: '" + command + "' argument: '" + argument + "'");
      }
      else if(result == REGEXP_NOMATCH)
      {
        Serial.println("Unknown command: '" + input + "'");
      }

      Serial.println("Mode: '" + String(mode) + "'");    
    }
  }

  switch(mode)
  {
    case BREATHE_MODE:
      BreatheMode(color, min_brightness, max_brightness, breath_speed, hold_breath_time);
      break;
    default:
      NullMode(50);
      break;
  }  
}

void NullMode(uint8_t sleep_ms)
{
    delay(sleep_ms);
}

void BreatheMode(uint32_t color, uint8_t min_brightness, uint8_t max_brightness, uint32_t breathe_speed, uint32_t hold_breath_ms)
{
    int brightness = 0;
    // breathe in...
    while(brightness < max_brightness)
    {
        strip.setBrightness(brightness);
        strip.show();
        delay(breath_speed);                
        brightness++;
    }
    // hold breath
    delay(hold_breath_time);

    while(brightness >= min_brightness)
    {
      strip.setBrightness(brightness);
      strip.show();
      brightness--;
      delay(breath_speed);
    }

    delay(hold_breath_time);
}