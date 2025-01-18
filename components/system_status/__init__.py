from esphome import automation
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@swoboda1337"]

ON_DUMP_CONFIG = "on_dump_config"

system_status_ns = cg.esphome_ns.namespace("system_status")
SystemStatusComponent = system_status_ns.class_("SystemStatusComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SystemStatusComponent),
        cv.Optional(ON_DUMP_CONFIG): automation.validate_automation(single=True),
    }
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
