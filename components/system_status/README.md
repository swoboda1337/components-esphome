# System status component

This component outputs useful debug when a logger connects to the device.

Custom logging can be added in the on_dump_config action.

Example:
```yaml
esphome:
  name: "test"
  on_boot:
    - system_status.set_integer:
        id: system_status_id
        label: "WiFi disconnects"
        value: 0

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  on_disconnect:
    - system_status.inc_integer:
        id: system_status_id
        label: "WiFi disconnects"
        value: 1

system_status:
  id: system_status_id
  on_dump_config:
    - system_status.set_string:
        id: system_status_id
        label: "Lambda"
        value: "asdf"
```

Logs:
```
[23:08:04][C][system_status:058]: System Status:
[23:08:04][C][system_status:062]:   Frequency: 240000000
[23:08:04][C][system_status:062]:   Lambda: 1234
[23:08:04][C][system_status:062]:   Uptime: 0 days 00:00:06
[23:08:05][C][system_status:062]:   WiFi disconnects: 0
```