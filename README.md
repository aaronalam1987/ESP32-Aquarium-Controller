# ESP32-Aquarium-Controller
An aquarium controller based on an ESP32.

This ESP32 based aquarium controller utilises a 20x4 LCD, DS18B20 temperature probe and an 8 channel relay to provide features such as:
  -Automatic heating and cooling control based on target temperature setting.
  -"Waterchange/Feed" mode, a simple press is all that is needed to enable or disable specified connected equipment.
  -Time based lighting control, start and end times are configurable and current real time is provided by the RTC.
  -Manual control of connected equipment, utilising an 8 channel relay connected equipment can be enabled or disabled by a simple button press.
  -WebServer, allowing web-based controls and temperature monitoring.
  -Automatic saving of all modifiable settings to the ESP32 eeprom.

The interface is navigatable through a series of menus providing a clear and simple way to control or view settings.