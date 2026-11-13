// vim:ft=cpp:
#pragma once

#include "esphome/components/remeha_thermostat/remeha_thermostat.h"
#include "esphome/core/component.h"
#include "esphome/components/select/select.h"
#include "esphome/components/number/number.h"

namespace esphome::remeha_thermostat {

class RemehaThermostatSchedule : public select::Select, public Component, public Parented<RemehaThermostat> {
public:
  void setup() {
    this->datapoint_->add_on_state_callback([this](float value) {
      this->on_datapoint_update_((size_t) value);
    });
  }

  void set_datapoint(number::Number* datapoint) {
    this->datapoint_ = datapoint;
  }

protected:
  void control(size_t index) override;
  void on_datapoint_update_(size_t);
  number::Number *datapoint_{nullptr};
};

} // esphome::remeha_thermostat
