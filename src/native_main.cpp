#ifndef ARDUINO
#include "ble_server.h"
#include <iostream>

int main() {
    std::cout << "Native stub executable for BLEServerManager" << std::endl;
    BLEServerManager::init();
    if (BLEServerManager::isConnected()) {
        std::cout << "Device connected" << std::endl;
    } else {
        std::cout << "Device not connected" << std::endl;
    }
    return 0;
}
#endif
