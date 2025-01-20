#include "sen0466.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
  namespace sen0466_sensor {

    static const char *const TAG = "sen0466_sensor.sensor";

    void Sen0466Sensor::setup() {
        
        CommandMessage command;
        command.header = COMMAND_HEADER;
        command.address = COMMAND_ADDRESS;
        command.data[0] = CMD_CHANGE_GET_METHOD;
        command.data[1] = SENSOR_MODE_PASSIVE;
        command.checksum = calculate_checksum((uint8_t *)&command, sizeof(command));

        ESP_LOGCONFIG(TAG, "Setting up sen0466...");
        if (!this->write_bytes(0, (uint8_t *)&command, sizeof(command), false)) {
            ESP_LOGW(TAG, "Error setting measurement mode.");
            this->mark_failed();
        };
    } 

    void Sen0466Sensor::dump_config() {
        ESP_LOGCONFIG(TAG, "DF Robot Carbon Monoxide Sensor sen0466:");
        LOG_I2C_DEVICE(this);
        if(this->is_failed()) {
            ESP_LOGE(TAG, "Communication with sen0466 failed!");
        }
        LOG_UPDATE_INTERVAL(this);
    }

    void Sen0466Sensor::update() { 
        ESP_LOGW(TAG, "Reading data from sen0466...");
        this->read_data_(); 
    } 

    void Sen0466Sensor::read_data_()
    {
      uint8_t result[9] = {0};
      uint8_t decimal_digits;
      this->read_bytes(0, result, sizeof(result));

      float gas_concentration = 0.0;
      if (calculate_checksum(result, 8) == result[8])
      {
        ESP_LOGW(TAG, "Checksum correct");
        gas_concentration= ((result[2] << 8) + result[3]) * 1.0;
        decimal_digits = result[5];
        switch(decimal_digits) {
          case 1:
            gas_concentration *= 0.1;
            break;
          case 2:
            gas_concentration *= 0.01;
            break;
          default:
            break;
        }

      } else {
        ESP_LOGE(TAG, "Sen0466 checksum error!");
        gas_concentration = 0.0;
      }
      if (gas_concentration < 0.00001) {
        gas_concentration = 0.0;
      }
      this->publish_state(gas_concentration);
    }

    uint8_t Sen0466Sensor::calculate_checksum(uint8_t* data, uint8_t length)
    {
      // Calculate the last byte checksum sent/received. 
      // Checksum = NOT(SUM bytes 1 through 7) + 1
      uint8_t j, checksum = 0;
      // Skip the first byte
      data += 1;
      // SUM bytes 1 through 7
      for (j = 0; j < (length - 2); j++) {
        checksum += *data;
        data++;
      }
      // Bitwise inverse and add 1
      checksum = (~checksum) + 1;
      return (checksum);
    } 
  }
}