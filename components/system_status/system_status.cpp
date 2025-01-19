#include "esphome/core/automation.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"
#include "system_status.h"
#ifdef USE_ESP32
#include <esp_timer.h>
#endif

namespace esphome {
namespace system_status {

static const char *const TAG = "system_status";

std::string SystemStatusComponent::get_uptime_() {
#ifdef USE_ESP32
  uint32_t seconds = esp_timer_get_time() / 1000000;
  uint32_t days = seconds / (60 * 60 * 24);
  seconds -= days * (60 * 60 * 24);
  uint32_t hours = seconds / (60 * 60);
  seconds -= hours * (60 * 60);
  uint32_t minutes = seconds / 60;
  seconds -= minutes * 60;
  return str_sprintf("%" PRIu32 " days %02" PRIu32 ":%02" PRIu32 ":%02" PRIu32,
                     days, hours, minutes, seconds);
#else
  return "";
#endif
}

void SystemStatusComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "System Status:");
  ESP_LOGCONFIG(TAG, "  Frequency: %" PRIu32 " hz", arch_get_cpu_freq_hz());
  ESP_LOGCONFIG(TAG, "  Uptime: %s", this->get_uptime_().c_str());
  this->dump_config_trigger_->trigger();
}

void SystemStatusComponent::setup() { }

}  // namespace system_status
}  // namespace esphome
