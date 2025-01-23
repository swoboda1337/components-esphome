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

void SystemStatusItem::set_units(std::string units) {
  this->units_str_ = " " + units;
}

void SystemStatusItem::set_string(std::string value) {
  this->is_string_ = true;
  this->value_str_ = value;
}

void SystemStatusItem::set_integer(int32_t value) {
  this->is_string_ = false;
  this->value_int_ = value;
}

void SystemStatusItem::inc_integer(int32_t value) {
  this->is_string_ = false;
  this->value_int_ += value;
}

std::string SystemStatusItem::to_string() {
  if (!this->is_string_) {
    this->value_str_ = str_sprintf("%" PRIi32, this->value_int_);
  }
  return this->value_str_ + this->units_str_;
}

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
  this->data_["Uptime"].set_string(this->get_uptime_());
  this->data_["Frequency"].set_integer(arch_get_cpu_freq_hz());
  this->dump_config_trigger_->trigger();
  ESP_LOGCONFIG(TAG, "System Status:");
  for (auto& item : this->data_) {
    std::string value = item.second.to_string();
    if (value != "") {
      ESP_LOGCONFIG(TAG, "  %s: %s", item.first.c_str(), value.c_str());
    }
  }
}

void SystemStatusComponent::setup() {
  this->data_["Frequency"].set_units("hz");
}

}  // namespace system_status
}  // namespace esphome
