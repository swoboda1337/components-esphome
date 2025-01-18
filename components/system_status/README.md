# System status component

This component outputs useful debug when a logger connects to the device.

Custom logging can be added in the on_dump_config action.

Example:
```yaml
system_status:
  on_dump_config:
    lambda: !lambda |-
      ESP_LOGCONFIG("system_status", "  Lambda: 1234");
```

Logs:
```
[23:05:31][C][system_status:032]: System Status:
[23:05:31][C][system_status:033]:   Frequency: 240000000 hz
[23:05:31][C][system_status:035]:   Uptime: 0 days 00:00:06
[23:05:31][C][system_status:056]:   Lambda: 1234
```