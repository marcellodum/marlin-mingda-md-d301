# marlin-mingda-rock-3-pro
Unofficial Marlin firmware for the Mingda Rock 3 Pro

# Installation

Copy on an empty SD card the TFT35 folder from the data folder and the firmware.bin file for your desired configuration. Insert the SD card, turn off the printer and turn it back on. The firmware and icons update process should start. Remember to remove the TFT35 folder from the SD card or the icons update process will restart each time you turn on the printer.

# UBL

When using UBL after completing and saving the mesh, your start gcode should be updated to enable UBL and load the correct saved mesh for example:

```
G28 ; home all axes
G29 A         ; Activate the UBL System.
G29 L0        ; Load the mesh stored in slot 0 (from G29 S0)
G29 J         ; Probe 3 points and tilt the mesh according to what it finds, optionally G29 J2 would do 4 points.
```

# Contribution

Any contribution is extremely welcome, please get in contact with me or submit a pull request.
