# LEGO LED Controller

A comprehensive Arduino-based LED control system for LEGO displays, featuring multiple operating modes and intelligent A/B section pairing for realistic lighting effects.

## 🏢 Project Overview

This project controls a 300-LED WS2812B strip to provide dynamic lighting for LEGO building displays in Fergus' office. The system features three distinct operating modes designed for different use cases, from testing and debugging to creating lifelike lighting patterns.

### Key Features

- **Multi-Mode Operation**: Three distinct modes for different lighting scenarios
- **A/B Section Pairing**: Automatic synchronization of paired lighting sections
- **Configurable Sections**: Easy-to-modify LED section definitions
- **Serial Monitoring**: Real-time status updates and debugging information
- **ESP32 Compatible**: Optimized for ESP32 microcontroller platform

## 🔧 Hardware Requirements

| Component | Specification | Notes |
|-----------|---------------|-------|
| **Microcontroller** | ESP32 | Any ESP32 development board |
| **LED Strip** | WS2812B (300 LEDs) | NeoPixel compatible |
| **Power Supply** | 5V, adequate amperage | Calculate based on LED count |
| **Data Connection** | GPIO 2 (ESP32) | Configurable in code |
| **Level Shifter** | 3.3V to 5V (optional) | Recommended for reliable data |

### Wiring Diagram

```
ESP32 GPIO 2 ──→ LED Strip Data In
5V Power ──→ LED Strip VCC
GND ──→ LED Strip GND & ESP32 GND
```

## 📁 Project Structure

```
Lego_LED_Controller/
├── LED_Test.ino          # Main Arduino sketch
├── README.md             # This documentation
└── LICENSE               # Project license
```

## 🎮 Operating Modes

### Mode 1: Clock Mode
**Purpose**: Sequential LED testing and position verification

- Lights LEDs sequentially from 0 to 300
- Every 10th LED displays in **red** as position markers
- All other LEDs display in **white**
- Configurable delay between LED activations
- Continuous cycling for strip testing

**Use Cases**: 
- LED strip continuity testing
- Position mapping and verification
- Initial hardware validation

### Mode 2: Living Mode
**Purpose**: Realistic building lighting with A/B pairing

- Random section activation with natural timing
- **A/B Section Pairing**: Sections ending with "A" and "B" activate together
- Configurable on/off time ranges (2-5 seconds on, 1-3 seconds off)
- Simulates realistic building occupancy patterns
- Ignores "Blank" sections (always off)

**Use Cases**:
- Normal display operation
- Realistic building lighting simulation
- Office ambiance lighting

### Mode 3: Debug Mode
**Purpose**: Systematic section testing with pairing verification

- Tests each section individually with A/B pairs
- **Blue** LED color for easy identification
- 5-second test duration per section
- Displays section names and LED ranges in serial output
- Skips blank sections automatically

**Use Cases**:
- Section mapping verification
- Troubleshooting specific LED ranges
- A/B pairing validation

## 🏗️ LED Section Configuration

The system divides the 300-LED strip into logical sections representing different parts of the LEGO buildings:

### Current Section Layout

| Section Name | LED Range | Type | Paired With |
|--------------|-----------|------|-------------|
| **Blank** | 0-10 | Inactive | None |
| **Marvel Top A** | 11-13 | Building | Marvel Top B |
| **Marvel Middle A** | 14-19 | Building | Marvel Middle B |
| **Marvel Ground** | 20-23 | Building | None |
| **Marvel Middle B** | 24-29 | Building | Marvel Middle A |
| **Marvel Top B** | 30-35 | Building | Marvel Top A |
| **Blank 2** | 36-63 | Inactive | None |
| **Museum Ground** | 64-76 | Building | None |
| **Museum Top** | 77-97 | Building | None |

### A/B Pairing System

The intelligent pairing system automatically synchronizes sections:

- **Marvel Top A** ↔ **Marvel Top B**: Upper floors light together
- **Marvel Middle A** ↔ **Marvel Middle B**: Middle floors light together
- Independent sections (Ground, Museum) operate individually
- Blank sections are always ignored

## ⚙️ Configuration

### Quick Setup

1. **Select Operating Mode**: Change `CURRENT_MODE` (1, 2, or 3)
2. **Adjust Hardware**: Modify `DATA_PIN`, `NUM_LEDS` if needed
3. **Customize Colors**: Update color definitions for each mode
4. **Modify Timing**: Adjust delays and timing parameters

### Configuration Parameters

#### Hardware Settings
```cpp
#define NUM_LEDS 300        // Total LED count
#define DATA_PIN 2          // ESP32 GPIO pin
#define LED_TYPE WS2812B    // LED strip type
#define COLOR_ORDER GRB     // Color order
```

#### Mode 1 (Clock) Settings
```cpp
#define MODE1_MAX_LED 300           // Maximum LED to test
#define MODE1_NORMAL_COLOR White    // Standard LED color
#define MODE1_TENTH_COLOR Red       // Position marker color
#define MODE1_DELAY 1000           // Delay between LEDs (ms)
```

#### Mode 2 (Living) Settings
```cpp
#define MODE2_SECTION_COLOR White   // Active section color
#define MODE2_MIN_ON_TIME 2000     // Min active time (ms)
#define MODE2_MAX_ON_TIME 5000     // Max active time (ms)
#define MODE2_MIN_OFF_TIME 1000    // Min inactive time (ms)
#define MODE2_MAX_OFF_TIME 3000    // Max inactive time (ms)
```

#### Mode 3 (Debug) Settings
```cpp
#define MODE3_SECTION_COLOR Blue    // Test color
#define MODE3_SECTION_DELAY 5000   // Test duration (ms)
#define MODE3_OFF_DELAY 1000       // Between-test delay (ms)
```

### Adding New Sections

To add or modify LED sections, edit the `sections[]` array:

```cpp
LEDSection sections[] = {
  {start_led, end_led, "Section Name"},
  // Add new sections here
};
```

**Naming Conventions**:
- Use `" A"` and `" B"` suffixes for paired sections
- Use `"Blank"` for inactive areas
- Keep names descriptive for debugging

## 🚀 Installation & Setup

### Prerequisites
- **Arduino IDE** with ESP32 board support
- **FastLED Library** (install via Library Manager)

### Installation Steps

1. **Clone Repository**
   ```bash
   git clone https://github.com/FergusKidd/Lego_LED_Controller.git
   cd Lego_LED_Controller
   ```

2. **Install FastLED Library**
   - Open Arduino IDE
   - Go to Tools → Manage Libraries
   - Search for "FastLED" and install

3. **Configure Hardware**
   - Connect LED strip to ESP32 GPIO 2
   - Ensure adequate 5V power supply
   - Connect common ground

4. **Upload Code**
   - Open `LED_Test.ino` in Arduino IDE
   - Select your ESP32 board and port
   - Upload the sketch

5. **Monitor Operation**
   - Open Serial Monitor (115200 baud)
   - Observe real-time status updates

## 🔍 Monitoring & Debugging

### Serial Output Features

The system provides comprehensive serial monitoring:

- **Startup Information**: Hardware configuration and mode selection
- **Section Listings**: All defined sections with LED ranges
- **Real-time Status**: Section activation/deactivation in Living Mode
- **Debug Information**: Detailed testing progress in Debug Mode
- **Error Messages**: Invalid mode selection and other issues

### Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| No LEDs lighting | Power/wiring issue | Check connections and power supply |
| Wrong colors | Color order setting | Try different `COLOR_ORDER` values |
| Erratic behavior | Data signal integrity | Add level shifter or reduce distance |
| Sections not pairing | Naming convention | Ensure " A" and " B" suffixes match |

## 🛠️ Customization

### Creating New Modes

To add a new operating mode:

1. Add mode configuration constants
2. Update `setup()` function to display new mode info
3. Add mode condition in `loop()`
4. Implement new mode function following existing patterns

### Modifying Pairing Logic

The `findPairedSection()` function can be modified to support different pairing schemes:
- Number-based pairing (e.g., "1A" ↔ "1B")
- Color-based pairing
- Complex multi-section groups

### Advanced Features

Consider adding:
- **Web Interface**: ESP32 WiFi capabilities for remote control
- **MQTT Integration**: Smart home integration
- **Sensor Triggers**: Motion or light sensors for automatic activation
- **Color Animations**: Fading, breathing, or color cycling effects

## 📊 Performance Considerations

- **Power Consumption**: 300 LEDs at full brightness ≈ 18A at 5V
- **Refresh Rate**: FastLED updates at ~400Hz for smooth operation
- **Memory Usage**: Minimal impact on ESP32 with current configuration
- **Timing Precision**: Non-blocking code ensures responsive operation

## 🤝 Contributing

Contributions are welcome! Please consider:

1. **Fork** the repository
2. **Create** a feature branch
3. **Test** thoroughly with hardware
4. **Document** any new features
5. **Submit** a pull request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Fergus Kidd**  
*LEGO enthusiast and lighting automation developer*

---

*For technical support or feature requests, please open an issue on GitHub.*
