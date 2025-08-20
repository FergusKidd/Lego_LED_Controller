#include <FastLED.h>

// ==== CONFIGURATION SECTION - EASY TO EDIT ====
#define NUM_LEDS 300     // Your full 300 LED strip
#define DATA_PIN 2       // GPIO 2 on ESP32
#define LED_TYPE WS2812B // Your LED strip type
#define COLOR_ORDER GRB  // GRB is correct

// MODE SELECTION (1, 2, or 3)
#define CURRENT_MODE 1   // Change this to 1, 2, or 3 to switch modes

// MODE 1 SETTINGS - Clock mode (0-300)
#define MODE1_MAX_LED 300        // Clock from 0 to this number
#define MODE1_NORMAL_COLOR CRGB::White  // Normal LED color
#define MODE1_TENTH_COLOR CRGB::Red     // Every 10th LED color (10, 20, 30, etc)
#define MODE1_DELAY 1000         // Delay between LEDs in milliseconds

// MODE 2 SETTINGS - Living mode sections
struct LEDSection {
  int start;
  int end;
  const char* name;
};

// Define your LED sections here - easy to edit!
LEDSection sections[] = {
  {0, 10, "Blank"},           // 0-10 blank (will stay off)
  {11, 13, "Marvel Top A"},   // 11-13 
  {30, 35, "Marvel Top B"},   // 30-35
  {14, 19, "Marvel Middle A"}, // 14-19
  {24, 29, "Marvel Middle B"}, // 24-29 (changed 30 to 29 to avoid overlap)
  {20, 23, "Marvel Ground"},  // 20-23
  {36, 63, "Blank 2"},        // 36-63 blank (changed from 35 to avoid overlap)
  {64, 76, "Museum Ground"},  // 64-76
  {77, 97, "Museum Top"}      // 77-97
};

#define NUM_SECTIONS (sizeof(sections) / sizeof(sections[0]))
#define MODE2_SECTION_COLOR CRGB::White  // Color for active sections
#define MODE2_MIN_ON_TIME 2000           // Minimum time section stays on (ms)
#define MODE2_MAX_ON_TIME 5000           // Maximum time section stays on (ms)
#define MODE2_MIN_OFF_TIME 1000          // Minimum time section stays off (ms)
#define MODE2_MAX_OFF_TIME 3000          // Maximum time section stays off (ms)

// MODE 3 SETTINGS - Debug mode (test each section)
#define MODE3_SECTION_COLOR CRGB::Blue   // Color for debugging sections
#define MODE3_SECTION_DELAY 5000         // How long each section stays on (ms)
#define MODE3_OFF_DELAY 1000             // Delay between sections (ms)

// ==== END CONFIGURATION SECTION ====

CRGB leds[NUM_LEDS];

void setup() {
  // Serial for debugging
  Serial.begin(115200);
  Serial.println("Starting ESP32 FastLED Multi-Mode System...");
  
  // Add a small delay before initializing
  delay(1000);
  
  // Initialize FastLED for ESP32
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // Set brightness higher for testing
  FastLED.setBrightness(100);  // Higher brightness for visibility
  
  // Clear all LEDs at startup
  FastLED.clear();
  FastLED.show();
  
  Serial.println("FastLED initialized");
  Serial.print("Data pin: ");
  Serial.println(DATA_PIN);
  Serial.print("Number of LEDs: ");
  Serial.println(NUM_LEDS);
  Serial.print("Current Mode: ");
  Serial.println(CURRENT_MODE);
  
  if(CURRENT_MODE == 1) {
    Serial.println("Mode 1: Clock mode (0-300 with red markers every 10th)");
  } else if(CURRENT_MODE == 2) {
    Serial.println("Mode 2: Living mode (random section activation)");
    Serial.println("Defined sections:");
    for(int i = 0; i < NUM_SECTIONS; i++) {
      Serial.print("  ");
      Serial.print(sections[i].name);
      Serial.print(": LEDs ");
      Serial.print(sections[i].start);
      Serial.print("-");
      Serial.println(sections[i].end);
    }
  } else if(CURRENT_MODE == 3) {
    Serial.println("Mode 3: Debug mode (test each section in order)");
    Serial.println("Will test sections in this order:");
    for(int i = 0; i < NUM_SECTIONS; i++) {
      if(strstr(sections[i].name, "Blank") == NULL) {
        Serial.print("  ");
        Serial.print(sections[i].name);
        Serial.print(": LEDs ");
        Serial.print(sections[i].start);
        Serial.print("-");
        Serial.println(sections[i].end);
      }
    }
  }
  
  // Initialize random seed
  randomSeed(analogRead(0));
}

void loop() {
  if(CURRENT_MODE == 1) {
    runMode1();
  } else if(CURRENT_MODE == 2) {
    runMode2();
  } else if(CURRENT_MODE == 3) {
    runMode3();
  } else {
    Serial.println("Invalid mode! Please set CURRENT_MODE to 1, 2, or 3");
    delay(5000);
  }
}

// MODE 1: Clock mode (0-100 with red markers)
void runMode1() {
  // Clear all LEDs
  FastLED.clear();
  FastLED.show();
  Serial.println("Starting Mode 1 cycle (0-300)");
  delay(1000);
  
  // Clock through LEDs 0 to MODE1_MAX_LED
  for(int i = 0; i <= MODE1_MAX_LED; i++) {
    // Clear all LEDs
    FastLED.clear();
    
    // Set color based on position
    if(i > 0 && i % 10 == 0) {
      // Every 10th LED (10, 20, 30, etc.) is red
      leds[i] = MODE1_TENTH_COLOR;
      Serial.print("LED ");
      Serial.print(i);
      Serial.println(" (RED marker)");
    } else {
      // Normal LEDs are white
      leds[i] = MODE1_NORMAL_COLOR;
      if(i % 10 == 0 || i == MODE1_MAX_LED) {
        Serial.print("LED ");
        Serial.println(i);
      }
    }
    
    // Update the LED strip
    FastLED.show();
    
    // Wait specified delay
    delay(MODE1_DELAY);
  }
  
  Serial.println("Mode 1 cycle complete! Restarting...");
  delay(2000);
}

// MODE 2: Living mode (random section activation)
void runMode2() {
  static bool sectionStates[NUM_SECTIONS];
  static unsigned long sectionTimers[NUM_SECTIONS];
  static unsigned long nextChangeTime[NUM_SECTIONS];
  static bool initialized = false;
  
  // Initialize on first run
  if(!initialized) {
    Serial.println("Initializing Mode 2 (Living mode)");
    for(int i = 0; i < NUM_SECTIONS; i++) {
      sectionStates[i] = false;
      sectionTimers[i] = millis();
      nextChangeTime[i] = millis() + random(MODE2_MIN_OFF_TIME, MODE2_MAX_OFF_TIME);
    }
    initialized = true;
  }
  
  unsigned long currentTime = millis();
  bool anyChange = false;
  
  // Check each section for state changes
  for(int i = 0; i < NUM_SECTIONS; i++) {
    // Skip blank sections
    if(strstr(sections[i].name, "Blank") != NULL) {
      continue;
    }
    
    if(currentTime >= nextChangeTime[i]) {
      // Time to change state
      sectionStates[i] = !sectionStates[i];
      anyChange = true;
      
      if(sectionStates[i]) {
        // Turning on
        Serial.print("Turning ON: ");
        Serial.println(sections[i].name);
        nextChangeTime[i] = currentTime + random(MODE2_MIN_ON_TIME, MODE2_MAX_ON_TIME);
      } else {
        // Turning off
        Serial.print("Turning OFF: ");
        Serial.println(sections[i].name);
        nextChangeTime[i] = currentTime + random(MODE2_MIN_OFF_TIME, MODE2_MAX_OFF_TIME);
      }
    }
  }
  
  // Update LEDs if any section changed
  if(anyChange) {
    FastLED.clear();
    
    for(int i = 0; i < NUM_SECTIONS; i++) {
      if(sectionStates[i] && strstr(sections[i].name, "Blank") == NULL) {
        // Turn on this section
        for(int led = sections[i].start; led <= sections[i].end; led++) {
          if(led < NUM_LEDS) {
            leds[led] = MODE2_SECTION_COLOR;
          }
        }
      }
    }
    
    FastLED.show();
  }
  
  delay(100); // Small delay for responsiveness
}

// MODE 3: Debug mode (test each section in order)
void runMode3() {
  Serial.println("Starting Mode 3 - Section Debug Cycle");
  
  // Go through each section in order
  for(int i = 0; i < NUM_SECTIONS; i++) {
    // Skip blank sections
    if(strstr(sections[i].name, "Blank") != NULL) {
      Serial.print("Skipping blank section: ");
      Serial.println(sections[i].name);
      continue;
    }
    
    // Clear all LEDs
    FastLED.clear();
    
    // Turn on current section
    Serial.print("Testing section: ");
    Serial.print(sections[i].name);
    Serial.print(" (LEDs ");
    Serial.print(sections[i].start);
    Serial.print("-");
    Serial.print(sections[i].end);
    Serial.println(")");
    
    // Light up the section
    for(int led = sections[i].start; led <= sections[i].end; led++) {
      if(led < NUM_LEDS) {
        leds[led] = MODE3_SECTION_COLOR;
      }
    }
    
    // Show the LEDs
    FastLED.show();
    
    // Keep section on for specified time
    delay(MODE3_SECTION_DELAY);
    
    // Turn off section
    FastLED.clear();
    FastLED.show();
    
    Serial.print("Section ");
    Serial.print(sections[i].name);
    Serial.println(" turned OFF");
    
    // Delay between sections
    delay(MODE3_OFF_DELAY);
  }
  
  Serial.println("All sections tested! Restarting debug cycle...");
  delay(2000);
}
