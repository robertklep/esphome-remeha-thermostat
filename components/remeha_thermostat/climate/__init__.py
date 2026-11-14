import esphome.codegen as cg
import esphome.config_validation as cv
from   esphome.components import climate, sensor, number, binary_sensor
from   .. import CONF_REMEHA_THERMOSTAT_ID, remeha_thermostat_ns, RemehaThermostat

CODEOWNERS   = [ "@robertklep" ]
DEPENDENCIES = [ "remeha_thermostat" ]

# config keys
CONF_CURRENT_TEMPERATURE_ID = "current_temperature"
CONF_TARGET_TEMPERATURE_ID  = "target_temperature"
CONF_HEATING_STATE_ID       = "heating_state"
CONF_GET                    = "get"
CONF_SET                    = "set"

RemehaThermostatClimate = remeha_thermostat_ns.class_(
    "RemehaThermostatClimate",
    climate.Climate,
    cg.Component,
    cg.Parented.template(RemehaThermostat)
)

# configuration schema
TARGET_TEMPERATURE_SCHEMA = cv.Any(
    cv.use_id(number.Number),
    cv.Schema({
        cv.Required(CONF_GET): cv.use_id(sensor.Sensor),
        cv.Required(CONF_SET): cv.use_id(number.Number),
    })
)

CONFIG_SCHEMA = climate.climate_schema(RemehaThermostatClimate).extend({
    cv.GenerateID(CONF_REMEHA_THERMOSTAT_ID): cv.use_id(RemehaThermostat),
    cv.Required(CONF_CURRENT_TEMPERATURE_ID): cv.use_id(sensor.Sensor),
    cv.Required(CONF_TARGET_TEMPERATURE_ID):  TARGET_TEMPERATURE_SCHEMA,
    cv.Optional(CONF_HEATING_STATE_ID):       cv.use_id(binary_sensor.BinarySensor),
})

async def to_code(config):
    var = await climate.new_climate(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_REMEHA_THERMOSTAT_ID])
    cg.add(var.set_current_temperature(await cg.get_variable(config[CONF_CURRENT_TEMPERATURE_ID])))

    target_config = config[CONF_TARGET_TEMPERATURE_ID]
    if isinstance(target_config, dict):
        target_getter = await cg.get_variable(target_config[CONF_GET])
        target_setter = await cg.get_variable(target_config[CONF_SET])
    else:
        target_getter = target_setter = await cg.get_variable(target_config)
    cg.add(var.set_target_temperature_getter(target_getter))
    cg.add(var.set_target_temperature_setter(target_setter))

    if CONF_HEATING_STATE_ID in config:
        cg.add(var.set_heating_state(await cg.get_variable(config[CONF_HEATING_STATE_ID])))
