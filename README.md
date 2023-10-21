# ESP32-Aquarium-Controller<br />
An aquarium controller based on an ESP32.><br /><br />

This ESP32 based aquarium controller utilises a 20x4 LCD, DS18B20 temperature probe and an 8 channel relay to provide features such as:<br /><br />

  - Automatic heating and cooling control based on target temperature setting.<br />
  - Waterchange/Feed mode, a simple press is all that is needed to enable or disable specified connected equipment.<br /> - TO BE IMPLEMENTED.
  - Time based lighting control, start and end times are configurable and current real time is provided by the RTC.<br /> - TO BE IMPLEMENTED.
  - Control of connected equipment by utilising an 8 channel relay equipment can be enabled or disabled by a simple button press.<br />
  - WebServer, allowing web-based controls and temperature monitoring.<br />
  - Automatic saving of all modifiable settings to the ESP32 EEPROM.<br />
  - Allows the usage of a slave device to wirelessly control connected relays.><br />

The interface is navigatable through a series of menus providing a clear and simple way to control or view settings.<br /><br />

Relays are able to be connected either directly to the master device and controlled locally or connected to a slave device and controlled wirelessly through ESP-NOW.<br />
No manual MAC assignment is required as the message is broadcast to all devices, manually specifying the slave MAC may be required if you utilise more than one slave device.<br />
