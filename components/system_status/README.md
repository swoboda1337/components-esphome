# System status component

This component outputs useful debug when a logger connects to the device.

Custom logging can be added in the on_dump_config action.

Example:
```yaml
esphome:
  name: "test"
  on_boot:
    - system_status.set_units:
        label: "Lambda"
        units: "asdf"

system_status:
  on_dump_config:
    - system_status.set_string:
        label: "Lambda"
        value: "1234"

something:
  on_something:
    - system_status.inc_integer:
        label: "Something"
        value: 1
```

Logs:
```
[23:08:04][C][system_status:058]: System Status:
[23:08:04][C][system_status:062]:   Frequency: 240000000 hz
[23:08:04][C][system_status:062]:   Lambda: 1234 asdf
[23:08:04][C][system_status:062]:   Something: 5
[23:08:04][C][system_status:062]:   Uptime: 01:12:55 up 0 days
[23:08:05][C][system_status:062]:   WiFi disconnects: 0
```