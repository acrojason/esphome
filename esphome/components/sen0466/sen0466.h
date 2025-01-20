#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

// ref:
// https://github.com/DFRobot/DFRobot_MultiGasSensor

namespace esphome {
  namespace sen0466_sensor {
    // Sensor Command constants
    static const uint8_t COMMAND_HEADER = 0xff;
    static const uint8_t COMMAND_ADDRESS = 0x01;
    
    // Sensor Mode
    static const uint8_t SENSOR_MODE_PASSIVE = 0x04;
    static const uint8_t SENSOR_MODE_INITIATIVE = 0x03;

    // Gas type
    static const uint8_t SENSOR_GAS_TYPE_CO = 0x04;

    // Sensor commands
    static const uint8_t CMD_CHANGE_GET_METHOD = 0x78;
    static const uint8_t CMD_CHANGE_I2C_ADDRESS = 0x92; 
    static const uint8_t CMD_GET_GAS_CONCENTRATION = 0x86;

    typedef struct {
      uint8_t header;
      uint8_t address;
      uint8_t data[6];
      uint8_t checksum;
    } CommandMessage;

    class Sen0466Sensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
      public:
        void update() override;
        void dump_config() override;
        void setup() override;
        uint8_t calculate_checksum(uint8_t *data, uint8_t length);
            
      protected:
        void read_data_();
        
    };  

  }  // namespace sen0466_sensor
}  // namespace esphome