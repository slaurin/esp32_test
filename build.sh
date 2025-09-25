#!/bin/bash

# ESP32S3 Project Build Script
# This script helps build the ESP32S3 project with proper error handling

set -e  # Exit on error

echo "=== ESP32S3 Build Script ==="
echo "Project: esp32s3_test"
echo "Target: ESP32S3"
echo

# Check if ESP-IDF is available
if [ -z "$IDF_PATH" ]; then
    echo "Error: ESP-IDF not found. Please source the ESP-IDF environment:"
    echo "  . \$HOME/esp/esp-idf/export.sh"
    exit 1
fi

echo "ESP-IDF Path: $IDF_PATH"
echo "IDF Version: $(idf.py --version 2>/dev/null || echo 'Unknown')"
echo

# Set target to ESP32S3
echo "Setting target to ESP32S3..."
idf.py set-target esp32s3

echo
echo "Building project..."
idf.py build

echo
echo "Build completed successfully!"
echo "To flash: idf.py -p <PORT> flash"
echo "To monitor: idf.py -p <PORT> monitor"
echo "To flash and monitor: idf.py -p <PORT> flash monitor"