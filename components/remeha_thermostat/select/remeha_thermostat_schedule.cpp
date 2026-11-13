#include "remeha_thermostat_schedule.h"
#include "esphome/core/log.h"

namespace esphome::remeha_thermostat {

void RemehaThermostatSchedule::control(size_t index) {
  this->datapoint_->make_call().set_value(index).perform();
  // select indices are 0-based, datapoint is 1-based
  this->publish_state(index + 1);
}

void RemehaThermostatSchedule::on_datapoint_update_(size_t index) {
  // select indices are 0-based, datapoint is 1-based
  this->publish_state(index - 1);
}

} // esphome::remeha_thermostat
