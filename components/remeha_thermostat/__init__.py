import esphome.codegen as cg
import esphome.config_validation as cv
from   esphome.components import climate, sensor, number, binary_sensor

# import Remeha BLE client
from esphome.components.remeha_confida import RemehaConfida

CODEOWNERS   = [ "@robertklep" ]
DEPENDENCIES = [ "remeha_confida" ]
AUTO_LOAD    = [ "climate" ]
MULTI_CONF   = True

remeha_thermostat_ns = cg.esphome_ns.namespace("remeha_thermostat")
RemehaThermostat     = remeha_thermostat_ns.class_("RemehaThermostat", cg.PollingComponent)

# config keys
CONF_REMEHA_CLIENT_ID       = "remeha_client"
CONF_CURRENT_TEMPERATURE_ID = "current_temperature"
CONF_TARGET_TEMPERATURE_ID  = "target_temperature"
CONF_HEAT_DEMAND_ID         = "heat_demand"

# configuration schema
CONFIG_SCHEMA = climate.climate_schema(RemehaThermostat).extend({
    cv.Required(CONF_REMEHA_CLIENT_ID):       cv.use_id(RemehaConfida),
    cv.Required(CONF_CURRENT_TEMPERATURE_ID): cv.use_id(sensor.Sensor),
    cv.Required(CONF_TARGET_TEMPERATURE_ID):  cv.use_id(number.Number),
    cv.Optional(CONF_HEAT_DEMAND_ID):         cv.use_id(binary_sensor.BinarySensor),
})

async def to_code(config):
    var = await climate.new_climate(config)
    remeha_client = await cg.get_variable(config[CONF_REMEHA_CLIENT_ID])
    cg.add(var.set_remeha_client(remeha_client))
    cg.add(var.set_current_temperature(await cg.get_variable(config[CONF_CURRENT_TEMPERATURE_ID])))
    cg.add(var.set_target_temperature(await cg.get_variable(config[CONF_TARGET_TEMPERATURE_ID])))
    if CONF_HEAT_DEMAND_ID in config:
        cg.add(var.set_heat_demand(await cg.get_variable(config[CONF_HEAT_DEMAND_ID])))
    await cg.register_component(var, config)
