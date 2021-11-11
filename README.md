# water-quality
*Code and design info for our water quality monitoring station as it develops*

Recently started updating code to combine the Atlas Scientific scrippts with Adafruit IO. For Atlas Scientific references, you can download the Arduino code from the Documents & Downloads section of the page for each sensor (<< not sure if this is still the case).

* **Atlas Scientific DO sensor**: https://atlas-scientific.com/probes/dissolved-oxygen-probe/

* **Atlas Scientific pH sensor**: https://atlas-scientific.com/probes/ph-probe/

* **Atlas Scientific temperature sensor**: https://atlas-scientific.com/probes/pt-1000-temperature-probe/

* **Dweeting code for Arduino that we repurposed**: https://www.hackster.io/javier-munoz-saez/esp8266-sending-data-to-an-online-deskboard-3e7e91

## challenges
Right now the setup needs both power and wifi. We would like to have a more flexible setup, so we could run it off a battery, maybe with a solar panel, and collect data on an SD card or even through GSM or LoRa. The easiest functional remote scenario with our current setup is to bring a phone as a hotspot and power it with a laptop. The fixed scenario, our next goal, is to arrange with someone who has a pier to let us hook it up to their power and wifi.
