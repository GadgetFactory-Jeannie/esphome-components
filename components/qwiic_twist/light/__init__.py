import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID
from .. import qwiic_twist_ns, QwiicTwist, CONF_QWIIC_TWIST

QwiicTwistRGB = qwiic_twist_ns.class_("QwiicTwistRGB", light.LightOutput, cg.Component)

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(QwiicTwistRGB),
        cv.GenerateID(CONF_QWIIC_TWIST): cv.use_id(QwiicTwist),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)
    qwiic_twist = await cg.get_variable(config[CONF_QWIIC_TWIST])
    cg.add(var.set_parent(qwiic_twist))
