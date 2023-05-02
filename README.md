| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# CH1116 OLED With LVGL in ESP-IDF Example

Project rewritten from [I2C OLED example (ESP-IDF v5.0)](https://github.com/espressif/esp-idf/tree/release/v5.0/examples/peripherals/lcd/i2c_oled)

This example shows how to make use of the CH1116 panel driver from `esp_lcd` component to facilitate the porting of LVGL library. In the end, example will display a scrolling text on the OLED screen.

## Recommended Tutorial

[LittlevGL on a Monochrome OLED | LVGL’s Blog](https://blog.lvgl.io/2019-05-06/oled)

## How to use the example

### Hardware Required

* An ESP development board
* An CH1116 OLED LCD, with I2C interface
* An USB cable for power supply and programming

### Hardware Connection

The connection between ESP Board and the LCD is as follows:

```
      ESP Board                       OLED LCD (I2C)
+------------------+              +-------------------+
|               GND+--------------+GND                |
|                  |              |                   |
|               3V3+--------------+VCC                |
|                  |              |                   |
|               SDA+--------------+SDA                |
|                  |              |                   |
|               SCL+--------------+SCL                |
+------------------+              +-------------------+
```

The GPIO number used by this example can be changed in [lvgl_example_main.c](main/ch1116.c). Please pay attention to the I2C hardware device address as well, you should refer to your module's spec and schematic to determine that address.

### Build and Flash

Run `idf.py -p PORT build flash monitor` to build, flash and monitor the project. A scrolling text will show up on the LCD as expected.

The first time you run `idf.py` for the example will cost extra time as the build system needs to address the component dependencies and downloads the missing components from registry into `managed_components` folder.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

### Example Output

```bash
...
I (0) cpu_start: Starting scheduler on APP CPU.
I (345) example: Initialize I2C bus
I (345) example: Install panel IO
I (345) example: Install CH1116 panel driver
I (455) example: Initialize LVGL library
I (455) example: Register display driver to LVGL
I (455) example: Install LVGL tick timer
I (455) example: Display LVGL Scroll Text
...
```

## Troubleshooting

For any technical queries, please open an [issue] (https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you soon.
