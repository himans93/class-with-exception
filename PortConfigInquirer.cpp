#include "PortConfigInquirer.h"

PortConfigInquirer::PortConfigInquirer(const std::string& devicePath)
    : m_devicePath(devicePath), m_fileHandle(-1) {}

PortConfigInquirer::~PortConfigInquirer() {
    if (m_fileHandle != -1) {
        closeDevice();
    }
}

bool PortConfigInquirer::openDevice() {
    m_fileHandle = open(m_devicePath.c_str(), O_RDWR);
    if (m_fileHandle == -1) {
        std::cerr << "Failed to open device: " << m_devicePath << std::endl;
        return false;
    }
    return true;
}

void PortConfigInquirer::closeDevice() {
    if (m_fileHandle != -1) {
        close(m_fileHandle);
        m_fileHandle = -1;
    }
}

bool PortConfigInquirer::devctlGetConfig(qcrm_port_config_t& config) {
    int ret = ::devctl(m_fileHandle, /* command */, &config, sizeof(config), nullptr);
    if (ret != EOK) {
        std::cerr << "devctl failed: " << strerror(ret) << std::endl;
        return false;
    }
    return true;
}

bool PortConfigInquirer::validateConfig(const qcrm_port_config_t& config) {
    // Example validation logic
    if (config.field1 < 0 || config.field2 < 0) {
        std::cerr << "Validation failed: invalid configuration values" << std::endl;
        return false;
    }
    return true;
}

void PortConfigInquirer::setDefaultConfig(qcrm_port_config_t& config) {
    // Set default configuration values
    config.field1 = 42; // Example default value
    config.field2 = 99; // Example default value
}

bool PortConfigInquirer::inquireConfig(qcrm_port_config_t& config, int retries) {
    for (int attempt = 0; attempt < retries; ++attempt) {
        if (openDevice()) {
            if (devctlGetConfig(config)) {
                if (validateConfig(config)) {
                    closeDevice();
                    return true;
                } else {
                    std::cerr << "Invalid config retrieved, setting default config." << std::endl;
                    setDefaultConfig(config);
                    closeDevice();
                    return true;
                }
            }
            closeDevice();
        }
        std::cerr << "Retrying to inquire config, attempt " << (attempt + 1) << std::endl;
    }
    std::cerr << "Failed to retrieve valid config after retries, setting default config." << std::endl;
    setDefaultConfig(config);
    return false;
}
