import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

# import Remeha BLE client
from esphome.components.remeha_confida import RemehaConfida

CODEOWNERS   = [ "@robertklep" ]
DEPENDENCIES = [ "remeha_confida" ]
MULTI_CONF   = False

remeha_thermostat_ns = cg.esphome_ns.namespace("remeha_thermostat")
RemehaThermostat     = remeha_thermostat_ns.class_("RemehaThermostat", cg.Component)

# config keys
CONF_REMEHA_CLIENT_ID = "remeha_client"
CONF_REMEHA_THERMOSTAT_ID = "remeha_thermostat_id"

# configuration schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID():                    cv.declare_id(RemehaThermostat),
    cv.Required(CONF_REMEHA_CLIENT_ID): cv.use_id(RemehaConfida),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    remeha_client = await cg.get_variable(config[CONF_REMEHA_CLIENT_ID])
    cg.add(var.set_remeha_client(remeha_client))
    await cg.register_component(var, config)
