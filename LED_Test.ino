#include <FastLED.h>

// ==== CONFIGURATION SECTION - EASY TO EDIT ====
#define NUM_LEDS 1       // Single LED
#define DATA_PIN 4       // Try GPIO 4 instead of GPIO 2
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
  Serial.begin(9600);  // Try slower baud rate for compatibility
  delay(2000); // Give serial time to initialize
  
  Serial.println("=== ESP32 LED Test Starting ===");
  Serial.println("If you see this, the ESP32 is working!");
  
  // Test if we can print to serial continuously
  for(int i = 0; i < 5; i++) {
    Serial.print("Startup test ");
    Serial.print(i + 1);
    Serial.println("/5");
    delay(500);
  }
  
  Serial.println("Initializing FastLED...");
  
  // Initialize FastLED for ESP32
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // Set brightness lower for testing
  FastLED.setBrightness(50);  // Reduced brightness for troubleshooting
  
  // Clear all LEDs at startup
  FastLED.clear();
  FastLED.show();
  
  Serial.println("FastLED initialized successfully!");
  Serial.print("Data pin: GPIO ");
  Serial.println(DATA_PIN);
  Serial.print("Number of LEDs: ");
  Serial.println(NUM_LEDS);
  Serial.print("Number of colors in cycle: ");
  Serial.println(NUM_COLORS);
  Serial.println("Starting color cycling in 3 seconds...");
  
  delay(3000);
  Serial.println("=== Starting main loop ===");
}

void loop() {
  static int loopCount = 0;
  loopCount++;
  
  Serial.print("=== Loop cycle ");
  Serial.print(loopCount);
  Serial.println(" starting ===");
  
  // Cycle through all colors
  for(int i = 0; i < NUM_COLORS; i++) {
    // Print what we're trying to do
    Serial.print("Setting LED to color ");
    Serial.print(i + 1);
    Serial.print("/");
    Serial.print(NUM_COLORS);
    Serial.print(": ");
    
    // Print color name
    if(colors[i] == CRGB::Red) Serial.print("Red");
    else if(colors[i] == CRGB::Green) Serial.print("Green");
    else if(colors[i] == CRGB::Blue) Serial.print("Blue");
    else if(colors[i] == CRGB::Yellow) Serial.print("Yellow");
    else if(colors[i] == CRGB::Cyan) Serial.print("Cyan");
    else if(colors[i] == CRGB::Magenta) Serial.print("Magenta");
    else if(colors[i] == CRGB::White) Serial.print("White");
    else if(colors[i] == CRGB::Orange) Serial.print("Orange");
    else if(colors[i] == CRGB::Purple) Serial.print("Purple");
    else if(colors[i] == CRGB::Pink) Serial.print("Pink");
    else Serial.print("Unknown");
    
    Serial.println();
    
    // Set the single LED to the current color
    leds[0] = colors[i];
    
    // Update the LED
    FastLED.show();
    
    Serial.println("LED updated, waiting...");
    
    // Wait before changing to next color
    delay(COLOR_DELAY);
  }
  
  Serial.println("Color cycle complete! Restarting in 2 seconds...");
  delay(2000);
}
