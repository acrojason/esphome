import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    DEVICE_CLASS_CARBON_MONOXIDE,
    ICON_MOLECULE_CO,
    STATE_CLASS_MEASUREMENT,
    UNIT_PARTS_PER_MILLION,
)

CODEOWNERS = ["@acrojason"]
DEPENDENCIES = ["i2c"]

sen0466_sensor_ns = cg.esphome_ns.namespace("sen0466_sensor")
Sen0466Sensor = sen0466_sensor_ns.class_(
    "Sen0466Sensor", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        Sen0466Sensor,
        unit_of_measurement=UNIT_PARTS_PER_MILLION,
        icon=ICON_MOLECULE_CO,
        device_class=DEVICE_CLASS_CARBON_MONOXIDE,
        accuracy_decimals=4,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.polling_component_schema("30s"))
    .extend(i2c.i2c_device_schema(0x74))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
