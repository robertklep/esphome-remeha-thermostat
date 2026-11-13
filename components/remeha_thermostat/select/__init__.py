import esphome.codegen as cg
import esphome.config_validation as cv
from   esphome.const      import CONF_TYPE
from   esphome.components import select, number
from   .. import CONF_REMEHA_THERMOSTAT_ID, CONF_DATAPOINT, remeha_thermostat_ns, RemehaThermostat

CODEOWNERS   = [ "@robertklep" ]
DEPENDENCIES = [ "remeha_thermostat" ]

ScheduleSelect = remeha_thermostat_ns.class_(
    "RemehaThermostatSchedule",
    select.Select,
    cg.Component,
    cg.Parented.template(RemehaThermostat)
)

# configuration keys
CONF_SCHEDULE = "schedule"
CONF_LABELS   = "labels"

# configuration schema
_THERMOSTAT_ID_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_REMEHA_THERMOSTAT_ID): cv.use_id(RemehaThermostat)
})

_SCHEDULE_SCHEMA = cv.Schema({
    cv.Required(CONF_DATAPOINT) : cv.use_id(number.Number),
    cv.Optional(CONF_LABELS) : cv.All(
        cv.ensure_list(cv.string_strict),
        cv.Length(max = 3),
        [ cv.Length(max = 64) ]
    ),
})

CONFIG_SCHEMA = cv.typed_schema({
    CONF_SCHEDULE: select.select_schema(ScheduleSelect).extend(_SCHEDULE_SCHEMA).extend(_THERMOSTAT_ID_SCHEMA),
}, key = CONF_TYPE)

async def to_code(config):
    # default labels to use for schedules
    labels = [ "Tijdprogramma 1", "Tijdprogramma 2", "Tijdprogramma 3" ]

    # let user override the schedule labels
    if overrides := config.get(CONF_LABELS):
        labels = overrides[:len(labels)] + labels[len(overrides):len(labels)]

    # select options for this select type
    options = {
        CONF_SCHEDULE: labels
    }[config[CONF_TYPE]]

    # instantiate and register
    var = await select.new_select(config, options = options)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_REMEHA_THERMOSTAT_ID])
    cg.add(var.set_datapoint(await cg.get_variable(config[CONF_DATAPOINT])))
