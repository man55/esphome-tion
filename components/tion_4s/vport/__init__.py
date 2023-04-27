import esphome.codegen as cg
from esphome.const import PLATFORM_ESP32
from ... import tion, vport  # pylint: disable=relative-beyond-top-level

CODEOWNERS = ["@dentra"]
ESP_PLATFORMS = [PLATFORM_ESP32]
DEPENDENCIES = ["ble_client"]
AUTO_LOAD = ["vport", "tion"]


Tion4sBleVPort = tion.tion_ns.class_("Tion4sBleVPort", cg.PollingComponent, vport.VPort)
Tion4sBleIO = tion.tion_ns.class_("Tion4sBleIO")

CONFIG_SCHEMA = tion.tion_vport_ble_schema(Tion4sBleVPort, Tion4sBleIO)


async def to_code(config):
    await tion.setup_tion_vport_ble(config)
