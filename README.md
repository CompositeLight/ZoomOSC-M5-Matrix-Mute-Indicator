
# ZoomOSC-M5-Matrix-Mute-Indicator
Turns an M5 Atom Matrix into a wifi muted/unmuted indicator, working with ZoomOSC

Important note: Use at your own risk - I'm a rookie programmer. Any good bits in this code have been magpied from the Genius of Joseph Adams, as I've used his M5 Tally Arbiter listener as a starting point.

# Setup:
1) Install ZoomOSC on your PC/Mac.
2) In ZoomOSC set the transmission IP to: 255.255.255.255 and the Transmission Port to: 54000.
3) In ZoomOSC Addtional Settings Subscribe to: All.
4) In Arduino, edit the sketch to insert your wifi SSID name and password.
5) Once uploaded, the M5 will attempt to connect to wifi on power up.


# Operation:
On power up the M5 will attempt to connect to wifi. You will see a blue cross until it connects.
On network connection it will respond to mute/unmute OSC commands sent from ZoomOSC.

Red means the mic is live.
Green means the mic is muted.

![IMG_2](https://user-images.githubusercontent.com/79208113/120725161-3c10b600-c4cd-11eb-811f-fc610bf91a5d.jpg)![IMG_1](https://user-images.githubusercontent.com/79208113/120725207-59458480-c4cd-11eb-93a8-6c275ddc85a9.jpg)


Hope this is helpful to someone. Good luck out there!
