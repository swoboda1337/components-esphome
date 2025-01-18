#include "esphome/core/automation.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"
#include "system_status.h"

namespace esphome {
namespace system_status {

static const char *const TAG = "system_status";

void SystemStatusComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "System Status:");
  ESP_LOGCONFIG(TAG, "  Frequency: %u", arch_get_cpu_freq_hz());
  this->dump_config_trigger_->trigger();
}

void SystemStatusComponent::setup() { }

}  // namespace system_status
}  // namespace esphome
