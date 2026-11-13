// vim:ft=cpp:
#pragma once

#include "esphome/components/remeha_thermostat/remeha_thermostat.h"
#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/number/number.h"

namespace esphome::remeha_thermostat {

class RemehaThermostatClimate : public climate::Climate, public Component, public Parented<RemehaThermostat> {
public:
  void setup() {
    this->current_->add_on_state_callback([this](float value) {
      this->on_update_current_(value);
    });
    this->target_->add_on_state_callback([this](float value) {
      this->on_update_target_(value);
    });
    if (this->heat_demand_ != nullptr) {
      this->heat_demand_->add_on_state_callback([this](bool value) {
        this->on_update_heat_demand_(value);
      });
    }
    // hard-coded to always heating
    this->mode = climate::CLIMATE_MODE_HEAT;
  }

  void set_current_temperature(sensor::Sensor* current) {
    this->current_ = current;
  }

  void set_target_temperature(number::Number* target) {
    this->target_ = target;
  }

  void set_heat_demand(binary_sensor::BinarySensor* heat_demand) {
    this->heat_demand_ = heat_demand;
  }

protected:
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;
  void on_update_current_(float);
  void on_update_target_(float);
  void on_update_heat_demand_(bool);

  number::Number *target_{nullptr};
  sensor::Sensor *current_{nullptr};
  binary_sensor::BinarySensor *heat_demand_{nullptr};
};

} // esphome::remeha_thermostat
