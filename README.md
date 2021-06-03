# ZoomOSC-M5-Matrix-Mute-Indicator
Turns an M5 Atom Matrix into a wifi muted/unmuted indicator, working with ZoomOSC

Important note: Use at your own risk - I'm a rookie programmer. Any good bits in this code have been magpied from the Genius of Joseph Adams, as I've used his M5 Tally Arbiter listener as a starting point.

# Setup:
1) Install ZoomOSC on your PC/Mac. Set the transmission IP to: 255.255.255.255 and the Transmission Port to: 54000. Subscribe to All.
2) In Ardunino, edit the code to insert your SSID name and password.
3) Once uploaded, the M5 will attempt to connect to wifi.


# Operation:
On power up the M5 will attempt to connect to wifi. You will see a blue cross until it connects.
On network connection it will respond to mute/unmute OSC commands sent from ZoomOSC.

Red means the mic is live.
Green means the mic is muted.

Hope this is helpful to someone. Good luck out there!
