# System status component

This component outputs useful debug when a logger connects to the device.

Custom logging can be added in the on_dump_config action.

Example:
```yaml
esphome:
  name: "test"
  on_boot:
    - system_status.set_integer:
        label: "WiFi disconnects"
        value: 0
    - system_status.set_units:
        label: "Lambda"
        units: "asdf"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  on_disconnect:
    - system_status.inc_integer:
        label: "WiFi disconnects"
        value: 1

system_status:
  on_dump_config:
    - system_status.set_string:
        label: "Lambda"
        value: "asdf"
```

Logs:
```
[23:08:04][C][system_status:058]: System Status:
[23:08:04][C][system_status:062]:   Frequency: 240000000 hz
[23:08:04][C][system_status:062]:   Lambda: 1234 asdf
[23:08:04][C][system_status:062]:   Uptime: 01:12:55 up 0 days
[23:08:05][C][system_status:062]:   WiFi disconnects: 0
```