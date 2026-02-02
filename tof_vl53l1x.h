#include "esphome.h"
#include <Wire.h>
#include <VL53L1X.h>

// Global sensor object
VL53L1X tof_sensor;
bool tof_initialized = false;

// 1. SETUP FUNCTION (Called once at boot)
void tof_setup() {
  // Initialize I2C manually on Pins 21 (SDA) and 22 (SCL)
  // This bypasses ESPHome's I2C manager entirely so there are no locks.
  Wire.begin(21, 22);
  Wire.setClock(400000); // 400kHz speed
  
  tof_sensor.setBus(&Wire);
  tof_sensor.setTimeout(500);
  
  if (!tof_sensor.init()) {
    ESP_LOGE("custom_tof", "Failed to detect VL53L1X! Check wiring.");
    return;
  }
  
  // Configure for Desk usage
  // Long mode is best for standing desks (up to 4m)
  tof_sensor.setDistanceMode(VL53L1X::Long);
  tof_sensor.setMeasurementTimingBudget(50000); // 50ms budget

  // Narrow the Field of View (FoV)
  // Default is 16x16. Setting to 4x4 (minimum) creates a tight laser beam
  // to avoid detecting chair legs or knees.
  tof_sensor.setROISize(4, 4);

  tof_sensor.startContinuous(50); // Measure every 50ms
  
  tof_initialized = true;
  ESP_LOGI("custom_tof", "VL53L1X initialized successfully!");
}

// 2. READ FUNCTION (Called by YAML loop)
float tof_read() {
  if (!tof_initialized) return NAN;

  // Read the sensor (returns millimeters)
  uint16_t mm = tof_sensor.read();
  
  if (tof_sensor.timeoutOccurred()) {
    ESP_LOGW("custom_tof", "Sensor Timeout!");
    return NAN;
  }
  
  // Convert to Meters
  return mm / 1000.0;
}