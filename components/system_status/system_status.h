#pragma once

#include "esphome/core/component.h"
#include <map>

namespace esphome {
namespace system_status {

class SystemStatusItem {
 public:
  void value_inc(int32_t value);
  void value_dec(int32_t value);
  void value_set(int32_t value);
  void value_set(std::string value);
  const std::string& to_string();
 protected:
  std::string value_str_;
  int32_t value_int_{0};
  bool is_string_{false};
};


class SystemStatusComponent : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void dump_config() override;
  void value_inc(std::string id, int32_t value) { this->data_[id].value_inc(value); }
  void value_dec(std::string id, int32_t value) { this->data_[id].value_dec(value); }
  void value_set(std::string id, int32_t value) { this->data_[id].value_set(value); }
  void value_set(std::string id, std::string value) { this->data_[id].value_set(value); }
  Trigger<> *get_dump_config_trigger() const { return this->dump_config_trigger_; };

 protected:
  std::string get_uptime_();
  std::map<std::string, SystemStatusItem> data_;
  Trigger<> *dump_config_trigger_{new Trigger<>()};
};

}  // namespace system_status
}  // namespace esphome
