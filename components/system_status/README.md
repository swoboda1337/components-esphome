# System status component

This component output useful debug when the logger connects to the device.

Custom logging can be added in the on_dump_config action.

Example:
```yaml
system_status:
  on_dump_config:
    lambda: !lambda |-
      ESP_LOGCONFIG("system_status", "  Testing: 1234");
```

Logs:
```
[22:14:21][C][system_status:013]: System Status:
[22:14:21][C][system_status:014]:   Frequency: 240000000 hz
[22:39:31][C][system_status:056]:   Testing: 1234

```