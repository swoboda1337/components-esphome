#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/system_status/system_status.h"

namespace esphome {
namespace system_status {

template<typename... Ts> class SystemStatusSetStringAction : public Action<Ts...> {
 public:
  SystemStatusSetStringAction(SystemStatusComponent *system_status) : system_status_(system_status) {}
  TEMPLATABLE_VALUE(std::string, label)
  TEMPLATABLE_VALUE(std::string, value)

  void play(Ts... x) override {
    this->system_status_->set_string(this->label_.value(x...), this->value_.value(x...));
  }

 protected:
  SystemStatusComponent *system_status_;
};

template<typename... Ts> class SystemStatusSetIntegerAction : public Action<Ts...> {
 public:
  SystemStatusSetIntegerAction(SystemStatusComponent *system_status) : system_status_(system_status) {}
  TEMPLATABLE_VALUE(std::string, label)
  TEMPLATABLE_VALUE(int32_t, value)

  void play(Ts... x) override {
    this->system_status_->set_integer(this->label_.value(x...), this->value_.value(x...));
  }

 protected:
  SystemStatusComponent *system_status_;
};

template<typename... Ts> class SystemStatusIncIntegerAction : public Action<Ts...> {
 public:
  SystemStatusIncIntegerAction(SystemStatusComponent *system_status) : system_status_(system_status) {}
  TEMPLATABLE_VALUE(std::string, label)
  TEMPLATABLE_VALUE(int32_t, value)

  void play(Ts... x) override {
    this->system_status_->inc_integer(this->label_.value(x...), this->value_.value(x...));
  }

 protected:
  SystemStatusComponent *system_status_;
};

}  // namespace system_status
}  // namespace esphome
