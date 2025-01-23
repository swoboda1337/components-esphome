#pragma once

#include "esphome/core/component.h"
#include <map>

namespace esphome {
namespace system_status {

class SystemStatusItem {
 public:
  void set_units(std::string units);
  void set_string(std::string value);
  void set_integer(int32_t value);
  void inc_integer(int32_t value);
  std::string to_string();
 protected:
  std::string units_str_;
  std::string value_str_;
  int32_t value_int_{0};
  bool is_string_{false};
};


class SystemStatusComponent : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void setup() override;
  void dump_config() override;
  void set_units(std::string label, std::string units) { this->data_[label].set_units(units); }
  void set_string(std::string label, std::string value) { this->data_[label].set_string(value); }
  void set_integer(std::string label, int32_t value) { this->data_[label].set_integer(value); }
  void inc_integer(std::string label, int32_t value) { this->data_[label].inc_integer(value); }
  Trigger<> *get_dump_config_trigger() const { return this->dump_config_trigger_; };

 protected:
  std::string get_uptime_();
  std::map<std::string, SystemStatusItem> data_;
  Trigger<> *dump_config_trigger_{new Trigger<>()};
};

}  // namespace system_status
}  // namespace esphome
