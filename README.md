# ESP32-S3 BLE GATT Server

A basic ESP32-S3 project implementing BLE (Bluetooth Low Energy) functionality using the NimBLE stack with GATT (Generic Attribute Profile) server capabilities.

## Features

- ESP32-S3 support with optimized configuration
- NimBLE Bluetooth Low Energy stack
- Basic GATT server with custom service and characteristic
- Read/Write/Notify/Indicate operations
- **Temperature Service** with Environmental Sensing Service (0x181A)
- **Temperature monitoring** with current, max, and min values
- **Configurable temperature units** (Celsius/Fahrenheit)
- **Automatic updates** every 30 seconds
- VSCode integration with PlatformIO
- Serial debugging and monitoring

## Hardware Requirements

- ESP32-S3 development board (ESP32-S3-DevKitC-1 or compatible)
- USB cable for programming and serial communication

## Software Requirements

- [VSCode](https://code.visualstudio.com/)
- [PlatformIO IDE Extension](https://platformio.org/install/ide-vscode)

## Setup Instructions

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd esp32_test
   ```

2. **Open in VSCode:**
   - Open VSCode
   - Open the `esp32_ble_project.code-workspace` file
   - Install the recommended PlatformIO IDE extension if prompted

3. **Build the project:**
   - Open the PlatformIO terminal in VSCode (Ctrl+Shift+`)
   - Run: `pio run`

4. **Upload to ESP32-S3:**
   - Connect your ESP32-S3 board via USB
   - Run: `pio run --target upload`

5. **Monitor serial output:**
   - Run: `pio device monitor`
   - Or use the PlatformIO Serial Monitor in VSCode

## BLE Service Details

### Custom Service UUID: 
`12345678-1234-1234-1234-123456789abc`

### Custom Characteristic UUID:
`87654321-4321-4321-4321-cba987654321`

### Environmental Sensing Service UUID:
`0000181A-0000-1000-8000-00805f9b34fb` (Standard BLE Environmental Sensing Service)

### Temperature Characteristics:
- **Current Temperature**: `00002A6E-0000-1000-8000-00805f9b34fb` (Read, Notify)
- **Max Temperature**: `00002A6F-0000-1000-8000-00805f9b34fb` (Read, Notify)
- **Min Temperature**: `00002A70-0000-1000-8000-00805f9b34fb` (Read, Notify)
- **Temperature Config**: `00002A71-0000-1000-8000-00805f9b34fb` (Read, Write)
  - Write `0` for Celsius
  - Write `1` for Fahrenheit

### Supported Operations:
- **Read**: Get current value from the characteristic
- **Write**: Send data to the characteristic
- **Notify**: Receive automatic updates when the value changes
- **Indicate**: Receive confirmed notifications

## Usage

1. **Flash and run** the code on your ESP32-S3
2. **Open serial monitor** to see debug output
3. **Connect using a BLE client** (smartphone app, computer, etc.)
   - Device name: `ESP32-S3-BLE-Device`
   - Look for the Environmental Sensing Service (0x181A) or custom service UUID
4. **Interact with the characteristics**:
   - Read to get current value
   - Write to send data to the device
   - Enable notifications to receive periodic updates
5. **Monitor temperature**:
   - Read temperature characteristics to get current, max, and min values
   - Temperature updates automatically every 30 seconds
   - Configure units by writing to the Temperature Config characteristic (0=Celsius, 1=Fahrenheit)
   - Temperature values are sent as signed 16-bit integers (multiply by 100, e.g., 2250 = 22.50°C)

## BLE Client Apps for Testing

### Android:
- nRF Connect for Mobile
- BLE Scanner

### iOS:
- LightBlue Explorer
- nRF Connect for Mobile

### Windows/Mac/Linux:
- nRF Connect for Desktop
- Bluetility (Mac)

## Project Structure

```
esp32_test/
├── .vscode/                    # VSCode configuration
│   └── settings.json
├── src/                        # Source code
│   └── main.cpp               # Main application
├── platformio.ini             # PlatformIO configuration
├── esp32_ble_project.code-workspace  # VSCode workspace
├── .gitignore                 # Git ignore rules
└── README.md                  # This file
```

## Configuration

The project is configured in `platformio.ini` with:
- **Board**: ESP32-S3-DevKitC-1
- **Framework**: ESP-IDF via Arduino
- **BLE Stack**: NimBLE (enabled, Bluedroid disabled)
- **Monitor Speed**: 115200 baud
- **Upload Speed**: 921600 baud

## Troubleshooting

1. **Build errors**: Ensure PlatformIO Core is up to date
2. **Upload issues**: Check USB cable and driver installation
3. **BLE not visible**: Verify the device is advertising and not connected
4. **Serial monitor issues**: Check baud rate (115200) and port selection

## Customization

To modify the BLE service:
1. Change UUIDs in `main.cpp`
2. Add more characteristics to the service
3. Implement custom read/write handlers
4. Add additional services

## License

This project is open source and available under the [MIT License](LICENSE).