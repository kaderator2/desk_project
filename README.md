# Standing Desk Controller (ESP32 + VL53L1X)

Minimal firmware to run a standing desk controller using an ESP32, a Pololu VL53L1X ToF sensor (tight ROI for desk height), and two optocoupler-driven relays for UP/DOWN control. Integrates with ESPHome and HomeAssistant. Automated height-based presets to come!

## Features
- VL53L1X long-range mode, 4x4 ROI, continuous 50ms sampling
- Touch inputs for physical UP/DOWN control (ESP32 touch pins)
- Home Assistant template sensor for desk height (meters)
- Template switches to "nudge" the desk up/down
- Manual I2C initialization to avoid ESPHome I2C manager locks

## Repository files
- `desk-test.yaml` — ESPHome configuration
- `tof_vl53l1x.h` — Custom C++ integration using pololu/VL53L1X library
- `README.md` — This file

## Hardware
- ESP32 Dev board
- VL53L1X ToF sensor (Pololu breakout recommended)
- 2 optocouplers or suitable relay drivers to interface desk controls
- Wiring and power for desk controller

## Pinout / Wiring
- VL53L1X:
    - SDA -> GPIO21
    - SCL -> GPIO22
    - (I2C initialized in code: Wire.begin(21,22))
- Desk relays / optocouplers:
    - UP relay -> GPIO25
    - DOWN relay -> GPIO26
- Touch control wires (physical touch pads):
    - Touch UP -> Touch Pin 32
    - Touch DOWN -> Touch Pin 33
    

![alt text](https://i.imgur.com/dVJLNq9.png)

## Software / Libraries
- ESPHome (YAML config)
- pololu/VL53L1X library (declared in `desk-test.yaml` libraries section)
- Custom code is included via `includes: - tof_vl53l1x.h`

Key runtime behavior in `tof_vl53l1x.h`:
- Initializes Wire at 400kHz and sets VL53L1X bus
- Sets Long distance mode, 50ms timing budget, 4x4 ROI
- Starts continuous measurements every 50ms
- Provides `tof_read()` returning meters (float) or NaN on timeout

## Building and Flashing
Prerequisite: ESPHome installed.

Example:
- Compile & upload over USB:
    - `esphome run desk-test.yaml`
- Or build binary:
    - `esphome compile desk-test.yaml`
    - Flash via your chosen tool (esptool, OTA, etc.)

## Home Assistant Integration
- `Desk Height` sensor (template sensor, unit m, 3 decimals)
- `Desk Nudge UP` / `Desk Nudge DOWN` template switches to pulse the relays
- Touch sensors appear as binary sensors in ESPHome integration

## Troubleshooting
- "Failed to detect VL53L1X" — check wiring, sensor power, I2C pins.
- Timeouts in logs — try increasing measurement timing budget or verify sensor placement/reflectivity.
- If touch sensors misbehave, adjust `threshold:` values in `desk-test.yaml`.

## Notes
- The custom code bypasses ESPHome I2C manager and manually initializes Wire on GPIO21/22 to avoid locking issues with long/blocking sensor reads.
- Measurements are in meters; long mode chosen for desk range.

## License & Credits
- Code snippets in repo are MIT-compatible unless otherwise noted.
- Uses Pololu VL53L1X Arduino library (declared in YAML).

Credits: project adapted for a standing desk using VL53L1X and ESPHome integration.
