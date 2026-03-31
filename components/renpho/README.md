# Renpho BLE Scale

Passive BLE weight reader for Renpho ES-CS20M (and likely other QN-Scale AABB protocol scales).

No BLE connection needed — reads weight from manufacturer data in advertisements.

## Usage

Add to your ESPHome config as a package:

```yaml
packages:
  renpho: github://swoboda1337/components-esphome/components/renpho/renpho.yaml@main

substitutions:
  renpho_mac: "AA:BB:CC:DD:EE:FF"  # Replace with your scale's MAC address
```

Requires an ESP32 with BLE support and `esp-idf` framework.

## Finding your scale's MAC address

Enable `esp32_ble_tracker` with debug logging and step on the scale. Look for advertisements with manufacturer data starting with `AA BB`.
