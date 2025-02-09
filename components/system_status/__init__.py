from esphome import automation
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_VALUE

CODEOWNERS = ["@swoboda1337"]

CONF_LABEL = "label"
CONF_UNITS = "units"
ON_DUMP_CONFIG = "on_dump_config"

system_status_ns = cg.esphome_ns.namespace("system_status")
SystemStatusComponent = system_status_ns.class_("SystemStatusComponent", cg.PollingComponent)
SystemStatusSetUnitsAction = system_status_ns.class_("SystemStatusSetUnitsAction", automation.Action)
SystemStatusSetStringAction = system_status_ns.class_("SystemStatusSetStringAction", automation.Action)
SystemStatusSetIntegerAction = system_status_ns.class_("SystemStatusSetIntegerAction", automation.Action)
SystemStatusIncIntegerAction = system_status_ns.class_("SystemStatusIncIntegerAction", automation.Action)


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SystemStatusComponent),
            cv.Optional(ON_DUMP_CONFIG): automation.validate_automation(single=True),
        }
    )
    .extend(cv.polling_component_schema("1d"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if ON_DUMP_CONFIG in config:
        await automation.build_automation(
            var.get_dump_config_trigger(),
            [],
            config[ON_DUMP_CONFIG]
        )


@automation.register_action(
    "system_status.set_units",
    SystemStatusSetUnitsAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(SystemStatusComponent),
            cv.Required(CONF_LABEL): cv.templatable(cv.string),
            cv.Required(CONF_UNITS): cv.templatable(cv.string),
        }
    ),
)
async def system_status_set_string_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    label = await cg.templatable(config[CONF_LABEL], args, cg.std_string)
    units = await cg.templatable(config[CONF_UNITS], args, cg.std_string)
    cg.add(var.set_label(label))
    cg.add(var.set_units(units))
    return var


@automation.register_action(
    "system_status.set_string",
    SystemStatusSetStringAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(SystemStatusComponent),
            cv.Required(CONF_LABEL): cv.templatable(cv.string),
            cv.Required(CONF_VALUE): cv.templatable(cv.string),
        }
    ),
)
async def system_status_set_string_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    label = await cg.templatable(config[CONF_LABEL], args, cg.std_string)
    value = await cg.templatable(config[CONF_VALUE], args, cg.std_string)
    cg.add(var.set_label(label))
    cg.add(var.set_value(value))
    return var


@automation.register_action(
    "system_status.set_integer",
    SystemStatusSetIntegerAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(SystemStatusComponent),
            cv.Required(CONF_LABEL): cv.templatable(cv.string),
            cv.Required(CONF_VALUE): cv.templatable(cv.int_),
        }
    ),
)
async def system_status_set_integer_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    label = await cg.templatable(config[CONF_LABEL], args, cg.std_string)
    value = await cg.templatable(config[CONF_VALUE], args, cg.int32)
    cg.add(var.set_label(label))
    cg.add(var.set_value(value))
    return var


@automation.register_action(
    "system_status.inc_integer",
    SystemStatusIncIntegerAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(SystemStatusComponent),
            cv.Required(CONF_LABEL): cv.templatable(cv.string),
            cv.Required(CONF_VALUE): cv.templatable(cv.int_),
        }
    ),
)
async def system_status_inc_integer_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    label = await cg.templatable(config[CONF_LABEL], args, cg.std_string)
    value = await cg.templatable(config[CONF_VALUE], args, cg.int32)
    cg.add(var.set_label(label))
    cg.add(var.set_value(value))
    return var
