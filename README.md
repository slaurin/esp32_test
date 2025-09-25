# ESP32S3 Test Project

This is a boilerplate project for ESP32S3 development using ESP-IDF framework.

## Project Structure

```
esp32s3_test/
├── CMakeLists.txt              # Main CMake build file
├── sdkconfig.defaults          # Default ESP32S3 configuration
├── partitions.csv              # Flash partition table
├── main/
│   ├── CMakeLists.txt          # Main component build file
│   └── main.c                  # Main application code
├── components/                 # Directory for custom components
└── README.md                   # This file
```

## Features

- ESP32S3 optimized configuration
- Simple LED blink example using GPIO 48
- FreeRTOS task management
- Logging functionality
- Ready-to-use partition table
- Custom components directory structure

## Prerequisites

1. **ESP-IDF**: Install ESP-IDF v4.4 or later
   ```bash
   # Follow the official installation guide:
   # https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/
   ```

2. **ESP32S3 Development Board**: Any ESP32S3 development board

## Quick Start

1. **Clone and navigate to the project**:
   ```bash
   cd esp32_test
   ```

2. **Set up ESP-IDF environment**:
   ```bash
   # Source the ESP-IDF environment (adjust path as needed)
   . $HOME/esp/esp-idf/export.sh
   ```

3. **Configure the project**:
   ```bash
   idf.py set-target esp32s3
   idf.py menuconfig  # Optional: modify configuration
   ```

4. **Build the project**:
   ```bash
   idf.py build
   ```

5. **Flash to ESP32S3**:
   ```bash
   idf.py -p /dev/ttyUSB0 flash  # Adjust port as needed
   ```

6. **Monitor output**:
   ```bash
   idf.py -p /dev/ttyUSB0 monitor
   ```

## Default Behavior

The default application:
- Initializes the ESP32S3
- Configures GPIO 48 as an output (built-in LED on many ESP32S3 boards)
- Blinks the LED every second
- Outputs log messages to the serial console

## Configuration

### GPIO Configuration
The default LED GPIO is set to GPIO 48. If your board uses a different GPIO for the built-in LED, modify the `BLINK_GPIO` definition in `main/main.c`:

```c
#define BLINK_GPIO GPIO_NUM_XX  // Replace XX with your GPIO number
```

### CPU Frequency
The project is configured to run at 240 MHz by default. You can change this in `sdkconfig.defaults` or through `idf.py menuconfig`.

### Partition Table
The default partition table includes:
- NVS (Non-Volatile Storage): 24KB
- PHY init data: 4KB  
- Application: 1MB

Modify `partitions.csv` if you need different partition sizes.

## Adding Custom Components

1. Create a new directory in `components/`:
   ```bash
   mkdir components/my_component
   ```

2. Create the component files:
   ```bash
   # components/my_component/CMakeLists.txt
   idf_component_register(SRCS "my_component.c"
                       INCLUDE_DIRS "include")
   
   # components/my_component/include/my_component.h
   # components/my_component/my_component.c
   ```

3. Include the component header in your main application:
   ```c
   #include "my_component.h"
   ```

## Troubleshooting

### Build Issues
- Ensure ESP-IDF is properly installed and sourced
- Check that `IDF_PATH` environment variable is set
- Verify target is set to ESP32S3: `idf.py set-target esp32s3`

### Flashing Issues
- Check the correct port: `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`
- Try different baud rates: `idf.py -p PORT -b 115200 flash`
- Press and hold BOOT button while flashing if needed

### LED Not Blinking
- Verify the correct GPIO number for your board's LED
- Some boards may have the LED on a different GPIO or may not have a built-in LED
- Check board documentation for the correct GPIO

## ESP32S3 Specific Features

This boilerplate is configured to take advantage of ESP32S3 features:
- Dual-core Xtensa LX7 CPU
- Wi-Fi and Bluetooth LE support (can be enabled in menuconfig)
- USB OTG support
- AI acceleration support
- Advanced peripheral set

## Next Steps

- Enable Wi-Fi: Add Wi-Fi configuration in menuconfig and implement Wi-Fi connection code
- Add Bluetooth: Enable Bluetooth in menuconfig and add BLE functionality  
- Use advanced peripherals: I2C, SPI, UART, PWM, etc.
- Implement OTA updates: Add OTA component for wireless firmware updates
- Add sensors: Connect and read from various sensors using I2C/SPI
- Web server: Create a simple web server for device control

## Resources

- [ESP32S3 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [ESP32S3 Hardware Design Guidelines](https://www.espressif.com/sites/default/files/documentation/esp32-s3_hardware_design_guidelines_en.pdf)