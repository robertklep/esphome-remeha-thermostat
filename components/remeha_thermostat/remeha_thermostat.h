// vim:ft=cpp:
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/remeha_confida/remeha_confida.h"

namespace esphome::remeha_thermostat {

static const char *const TAG = "remeha_thermostat";

class RemehaThermostat : public climate::Climate, public Component {
public:
  void setup() {
    this->current_->add_on_state_callback([this](float value) {
      this->update_current_(value);
    });
    this->target_->add_on_state_callback([this](float value) {
      this->update_target_(value);
    });
    if (this->heat_demand_ != nullptr) {
      this->heat_demand_->add_on_state_callback([this](bool value) {
        this->update_heat_demand_(value);
      });
    }
    // hard-coded to always heating
    this->mode = climate::CLIMATE_MODE_HEAT;
  }

  void set_remeha_client(remeha_confida::RemehaConfida *client) {
    this->client_ = client;
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
  void update_current_(float);
  void update_target_(float);
  void update_heat_demand_(bool);

  remeha_confida::RemehaConfida *client_{nullptr};
  number::Number *target_{nullptr};
  sensor::Sensor *current_{nullptr};
  binary_sensor::BinarySensor *heat_demand_{nullptr};
};

} // esphome::remeha_thermostat
