#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace system_status {

class SystemStatusComponent : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void setup() override;
  void dump_config() override;
  Trigger<> *get_dump_config_trigger() const { return this->dump_config_trigger_; };

 protected:
  Trigger<> *dump_config_trigger_{new Trigger<>()};
  std::string get_uptime_();
};

}  // namespace system_status
}  // namespace esphome
