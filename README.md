# marlin-mingda-md-d301
Unofficial Marlin firmware for the Mingda MD D301 board used on the Rock 3, Rock 3 Pro, D2, and other printers.
From version 1.0 forward, I'll be preparing firmware binaries only using the Marlin LVGL UI, if you want to use the legacy Mingda UI or any other UI you can change it in the Configuration.h file and compile the firmware back.
# Installation (Marlin LVGL UI)
Uncompress assets.zip and the desired firmware zip file and copy them on an empty SD card:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/sd_card_content.png)

Turn off the printer, insert the SD card, and turn it back on. The firmware and icons update process should start.

# Enabling 3d Preview using Cura Slicer (Marlin LVGL UI)
The following process has been tested on Cura version 4.11.0. An Ultimaker account will be needed, it is free and you can create it when adding the MKS Wifi plugin to Cura. Also, remember to sign in to your account from inside Cura.

Open https://marketplace.ultimaker.com/app/cura/plugins/Jeredian/MKSWifiPlugin and click the "Add to Cura" button:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_add_to_cura.png)

Open Cura and when/if asked syncronize your account data, when "Changes from your account" dialog appeats click the "Next" button:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_cura_sync.png)

Complete the installation steps and restart Cura.
From the Cura printers list click the "Manage printers" button:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_manage_printers.png)

Select your Mingda printer and click the "MKS WiFi Plugin" button:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_select_printer.png)

Check "MKS WiFi Plugin is active for this printer" and click on the "Preview settings" tab:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_active_for_printer.png)

Check "Screenshot support", select "Custom" as "Printer model", set 100 as "Simage" and 200 as "Gimage" and click the "Close" button for this dialog and for the preferences:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_preview_settings.png)

After slicing your model as usual, click on the "Save to Disk" split button:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_slice_split_button.png)

From the dropdown select "Save as TFT file" and proceed as usual:

![](https://github.com/marcellodum/marlin-mingda-md-d301/blob/master/docs/images/mks_wifi_plugin_save_as_tft_file.png)

# Legacy Installation (Mingda UI)

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
