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

    this->target_getter_->add_on_state_callback([this](float value) {
      this->on_update_target_(value);
    });

    if (this->heating_state_ != nullptr) {
      this->heating_state_->add_on_state_callback([this](bool value) {
        this->on_update_heating_state_(value);
      });
    }
    // hard-coded to always heating
    this->mode = climate::CLIMATE_MODE_HEAT;
  }

  void set_current_temperature(sensor::Sensor* current) {
    this->current_ = current;
  }

  void set_target_temperature_getter(sensor::Sensor* getter) {
    this->target_getter_ = getter;
  }

  void set_target_temperature_setter(number::Number* setter) {
    this->target_setter_ = setter;
  }

  void set_heating_state(binary_sensor::BinarySensor* state) {
    this->heating_state_ = state;
  }

protected:
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;
  void on_update_current_(float);
  void on_update_target_(float);
  void on_update_heating_state_(bool);

  sensor::Sensor *current_{nullptr};
  sensor::Sensor *target_getter_{nullptr};
  number::Number *target_setter_{nullptr};
  binary_sensor::BinarySensor *heating_state_{nullptr};
};

} // esphome::remeha_thermostat
