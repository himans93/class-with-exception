#include <iostream>
#include "PortConfigInquirer.h"

int main() {
    std::string devicePath = "/dev/some_device";
    qcrm_port_config_t config;

    PortConfigInquirer inquirer(devicePath);
    if (inquirer.inquireConfig(config)) {
        std::cout << "Configuration inquiry successful or default config set" << std::endl;
        // Process the config
    } else {
        std::cerr << "Configuration inquiry failed after retries, default config set" << std::endl;
    }

    // Print config values to verify
    std::cout << "Config field1: " << config.field1 << std::endl;
    std::cout << "Config field2: " << config.field2 << std::endl;

    return 0;
}
