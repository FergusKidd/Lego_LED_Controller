#include <FastLED.h>

// ==== CONFIGURATION SECTION - EASY TO EDIT ====
#define NUM_LEDS 1       // Single LED
#define DATA_PIN 2       // GPIO 2 on ESP32
#define LED_TYPE WS2812B // Your LED strip type
#define COLOR_ORDER GRB  // GRB is correct

// COLOR CYCLING SETTINGS
#define COLOR_DELAY 1000 // Delay between color changes in milliseconds

// Define the colors to cycle through
CRGB colors[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Yellow,
  CRGB::Cyan,
  CRGB::Magenta,
  CRGB::White,
  CRGB::Orange,
  CRGB::Purple,
  CRGB::Pink
};

#define NUM_COLORS (sizeof(colors) / sizeof(colors[0]))

// ==== END CONFIGURATION SECTION ====

CRGB leds[NUM_LEDS];

void setup() {
  // Serial for debugging
  Serial.begin(115200);
  Serial.println("Starting ESP32 Single LED Color Cycling...");
  
  // Add a small delay before initializing
  delay(1000);
  
  // Initialize FastLED for ESP32
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // Set brightness
  FastLED.setBrightness(100);
  
  // Clear all LEDs at startup
  FastLED.clear();
  FastLED.show();
  
  Serial.println("FastLED initialized");
  Serial.print("Data pin: GPIO ");
  Serial.println(DATA_PIN);
  Serial.print("Number of LEDs: ");
  Serial.println(NUM_LEDS);
  Serial.print("Number of colors in cycle: ");
  Serial.println(NUM_COLORS);
  Serial.println("Starting color cycling...");
}

void loop() {
  // Cycle through all colors
  for(int i = 0; i < NUM_COLORS; i++) {
    // Set the single LED to the current color
    leds[0] = colors[i];
    
    // Update the LED
    FastLED.show();
    
    // Print the current color to serial
    Serial.print("Color ");
    Serial.print(i + 1);
    Serial.print("/");
    Serial.print(NUM_COLORS);
    Serial.print(": ");
    
    // Print color name based on RGB values
    if(colors[i] == CRGB::Red) Serial.println("Red");
    else if(colors[i] == CRGB::Green) Serial.println("Green");
    else if(colors[i] == CRGB::Blue) Serial.println("Blue");
    else if(colors[i] == CRGB::Yellow) Serial.println("Yellow");
    else if(colors[i] == CRGB::Cyan) Serial.println("Cyan");
    else if(colors[i] == CRGB::Magenta) Serial.println("Magenta");
    else if(colors[i] == CRGB::White) Serial.println("White");
    else if(colors[i] == CRGB::Orange) Serial.println("Orange");
    else if(colors[i] == CRGB::Purple) Serial.println("Purple");
    else if(colors[i] == CRGB::Pink) Serial.println("Pink");
    else Serial.println("Unknown");
    
    // Wait before changing to next color
    delay(COLOR_DELAY);
  }
  
  Serial.println("Color cycle complete! Restarting...");
  delay(1000);
}
