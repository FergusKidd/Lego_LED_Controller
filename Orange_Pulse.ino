#include <FastLED.h>

// ==== CONFIGURATION SECTION - EASY TO EDIT ====
#define NUM_LEDS 10      // 10 LEDs
#define DATA_PIN 2       // GPIO 2 on ESP32
#define LED_TYPE WS2812B // Your LED strip type
#define COLOR_ORDER GRB  // GRB is correct

// PULSING SETTINGS
#define PULSE_SPEED 3    // How fast the pulse cycles (lower = slower) - very slow fade
#define MIN_BRIGHTNESS 5   // Minimum brightness (0-255) - very dim
#define MAX_BRIGHTNESS 255 // Maximum brightness (0-255) - full brightness

// ORANGE COLOR DEFINITION
#define ORANGE_HUE 25    // Orange hue (0-255, where 25 is a warm orange)
#define ORANGE_SAT 255   // Full saturation for vibrant color
#define ORANGE_VAL 255   // Full value (brightness will be controlled separately)

// ==== END CONFIGURATION SECTION ====

CRGB leds[NUM_LEDS];
uint8_t gHue = ORANGE_HUE;  // Orange hue
uint8_t brightness = 0;     // Current brightness level
int8_t direction = 1;       // Direction of pulse (1 = getting brighter, -1 = getting dimmer)

void setup() {
  // Serial for debugging
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("=== Orange Pulsating LED Controller ===");
  Serial.println("10 LEDs with smooth orange glow effect");
  
  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // Clear all LEDs at startup
  FastLED.clear();
  FastLED.show();
  
  Serial.println("FastLED initialized");
  Serial.print("Data pin: GPIO ");
  Serial.println(DATA_PIN);
  Serial.print("Number of LEDs: ");
  Serial.println(NUM_LEDS);
  Serial.println("Starting orange pulse effect...");
  
  delay(1000);
}

void loop() {
  // Calculate smooth pulsing brightness using sine wave
  static uint8_t pulseCounter = 0;
  pulseCounter += PULSE_SPEED;
  
  // Use sine wave for smooth pulsing (sin8 returns 0-255)
  uint8_t pulseBrightness = sin8(pulseCounter);
  
  // Map the sine wave to our desired brightness range
  uint8_t mappedBrightness = map(pulseBrightness, 0, 255, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  
  // Set all LEDs to orange with the pulsing brightness
  CRGB orangeColor = CHSV(ORANGE_HUE, ORANGE_SAT, mappedBrightness);
  
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = orangeColor;
  }
  
  // Update the LED strip
  FastLED.show();
  
  // Slower frame rate for gentle, smooth fading
  delay(50);  // 20 FPS - smooth but not too fast
}