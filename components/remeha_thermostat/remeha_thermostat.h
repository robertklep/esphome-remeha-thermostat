// vim:ft=cpp:
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remeha_confida/remeha_confida.h"

namespace esphome::remeha_thermostat {

static const char *const TAG = "remeha_thermostat";

class RemehaThermostat : public Component {
public:
  void setup() {
  }

  void set_remeha_client(remeha_confida::RemehaConfida *client) {
    this->client_ = client;
  }

protected:
  remeha_confida::RemehaConfida *client_{nullptr};
};

} // esphome::remeha_thermostat
