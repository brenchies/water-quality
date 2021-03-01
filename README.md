# water-quality
Code and design info for our water quality monitoring station as it develops

Currently, the Arduino code is a mash-up of scripts from Atlas Scientific and Dweet.io, with references linked below. For Atlas Scientific references, you can download the Arduino code from the Documents & Downloads section of the page for each sensor.

Atlas Scientific DO sensor: https://atlas-scientific.com/probes/dissolved-oxygen-probe/
Atlas Scientific pH sensor: https://atlas-scientific.com/probes/ph-probe/
Atlas Scientific temperature sensor: https://atlas-scientific.com/probes/pt-1000-temperature-probe/

# challenges
Right now the setup needs both power and wifi. We would like to have a more flexible setup, so we could run it off a battery, maybe with a solar panel, and collect data on an SD card or even through GSM or LoRa. The easiest functional scenario with our current setup is to bring a phone as a hotspot and power it with a laptop. The next step up would be to get someone with a pier to let us hook it up to their power and wifi.
