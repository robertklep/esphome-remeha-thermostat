#include "remeha_thermostat_climate.h"
#include "esphome/core/log.h"

namespace esphome::remeha_thermostat {

climate::ClimateTraits RemehaThermostatClimate::traits() {
  auto traits = climate::ClimateTraits();
  // TODO: find out if device supports cooling
  traits.set_supported_modes({ climate::CLIMATE_MODE_HEAT });
  traits.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
  if (this->heat_demand_ != nullptr) {
    traits.add_feature_flags(climate::CLIMATE_SUPPORTS_ACTION);
  }
  // TODO: retrieve these from the device somehow?
  traits.set_visual_min_temperature(5.0f);
  traits.set_visual_max_temperature(30.0f);
  traits.set_visual_temperature_step(0.5f);
  return traits;
}

void RemehaThermostatClimate::control(const climate::ClimateCall &call) {
  bool updated = false;
  if (call.get_target_temperature().has_value()) {
    auto value = *call.get_target_temperature();
    ESP_LOGI(TAG, "Target temperature got set to %.2f°C", value);
    this->target_temperature = value;
    this->target_->make_call().set_value(value).perform();
    updated = true;
  }
  if (updated) {
    this->publish_state();
  }
}

void RemehaThermostatClimate::on_update_current_(float value) {
  ESP_LOGI(TAG, "Current temperature got updated to %.2f°C", value);
  this->current_temperature = value;
  this->publish_state();
}

void RemehaThermostatClimate::on_update_target_(float value) {
  ESP_LOGI(TAG, "Target temperature got updated to %.2f°C", value);
  this->target_temperature = value;
  this->publish_state();
}

void RemehaThermostatClimate::on_update_heat_demand_(bool value) {
  ESP_LOGI(TAG, "Heat demand got updated to %s", value ? "ON" : "OFF");
  this->action = value ? climate::CLIMATE_ACTION_HEATING : climate::CLIMATE_ACTION_IDLE;
  this->publish_state();
}

} // esphome::remeha_thermostat
