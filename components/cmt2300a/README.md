Example:
```yaml
esphome:
  name: "test"
  includes:
    - "/home/jon/Work/components-esphome/components/cmt2300a/cmt2300a.h"

spi:
  mosi_pin: GPIO09
  miso_pin: GPIO08
  clk_pin: GPIO07
  
sx126x:
  id: sx126x_id
  dio1_pin: GPIO39   
  busy_pin: GPIO40
  rst_pin: GPIO42
  cs_pin: GPIO41
  pa_power: 14
  bandwidth: 117_3kHz
  frequency: 865000000
  modulation: FSK
  bitrate: 20000
  shaping: GAUSSIAN_BT_0_5
  deviation: 20000
  hw_version: sx1262
  rf_switch: true
  sync_value: [0x4D, 0x48, 0x5A, 0x48]
  preamble_size: 30
  preamble_detect: 2
  rx_start: true
  tcxo_voltage: 1_8V
  tcxo_delay: 5ms
  payload_length: 60
  on_packet:
    then:
      - lambda: !lambda |-
          std::vector<uint8_t> decoded;
          uint32_t errors = CMT2300A::decode(x, decoded);
          ESP_LOGD("lambda", "original %s", format_hex(x).c_str());
          ESP_LOGD("lambda", "decoded  %s, errors %d", format_hex(decoded).c_str(), errors);

button:
  - platform: template
    name: "Transmit CMT2300"
    on_press:
      then:
        - lambda: |-
            std::vector<uint8_t> encoded;
            std::vector<uint8_t> data = {
                0x95, 0xA0, 0x11, 0x21, 0xAE, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
                0x00, 0x00, 0x01, 0x10, 0x00, 0x01, 0x00, 0x55, 0x00, 0x00, 0x00, 0xE8,
                0x00, 0x00, 0x06
            };
            CMT2300A::encode(data, encoded);
            id(sx126x)->send_packet(encoded);
```

