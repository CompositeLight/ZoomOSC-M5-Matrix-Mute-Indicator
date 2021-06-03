/* M5 Atom Matrix ZoomOSC Mute Indicator / Tally Light. Requires ZoomOSC (free version).
Large sections of this code (the good bits) have been borrowed/stolen/magpied from the work of genius Joseph Adams */

#include <M5Atom.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <PinButton.h>
#include <stdint.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#define DATA_PIN_LED 27
using namespace std;


// --------------------------------------------------------------------

/* USER CONFIG VARIABLES
    Change the following variables before compiling and sending the code to your device.
*/

//Wifi SSID and password
const char * networkSSID = "Your SSID Goes Here";
const char * networkPass = "pAsSwOrD";

//OSC Settings
const unsigned int localPort = 54000;   // local port to listen for OSC messages from ZoomOSC (must match the Transmission Port setting in ZoomOSC)


// --------------------------------------------------------------------


//General Variables
bool networkConnected = false;    // State of wifi connection
bool micMuted = false;        // Whether the mic is muted currently
uint8_t FSM = 0;


OSCErrorCode error;

//M5 variables
PinButton btnAction(39); //the "Action" button on the device - not currently used


//default color values
int GRB_COLOR_WHITE = 0xffffff;
int GRB_COLOR_BLACK = 0x000000;
int GRB_COLOR_RED = 0x00ff00;
int GRB_COLOR_ORANGE = 0xa5ff00;
int GRB_COLOR_YELLOW = 0xffff00;
int GRB_COLOR_GREEN = 0xff0000;
int GRB_COLOR_BLUE = 0x0000ff;
int GRB_COLOR_PURPLE = 0x008080;

int miclivecolour[] = {GRB_COLOR_RED, GRB_COLOR_BLACK};
int mutedcolour[] = {GRB_COLOR_GREEN, GRB_COLOR_BLACK};
int alloffcolor[] = {GRB_COLOR_BLACK, GRB_COLOR_BLACK};
int wificolor[] = {GRB_COLOR_BLUE, GRB_COLOR_BLACK};

int currentBrightness = 20;

//this is the array that stores the different LED looks
int number[19][25] = {{
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
  },
  { 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
  },
  { 1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 1, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1
  },
  { 0, 1, 1, 1, 0,
    1, 0, 1, 0, 1,
    1, 1, 0, 1, 1,
    1, 0, 1, 0, 1,
    0, 1, 1, 1, 0
  },

};


WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("Network connected!");
      // Serial.println(String(WiFi.localIP()));
      Serial.println(WiFi.localIP().toString());
      networkConnected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("Network connection lost!");
      networkConnected = false;
      break;
  }
}

//---------------------------------------------------------------
// HERE IS THE MAIN LED DRAWING ROUTINE aka drawNumber
void drawNumber(int arr[], int colors[])
{
  for (int i = 0; i < 25; i++)
  {
    M5.dis.drawpix(i, colors[arr[i]]);
  }
}
//---------------------------------------------------------------
//Change colour of LEDs

void greensquare(){
  M5.dis.clear();
  drawNumber(number[0], mutedcolour);
}

void redsquare(){
  M5.dis.clear();
  drawNumber(number[0], miclivecolour);
}

void wifiErrorSquare(){
  M5.dis.clear();
  drawNumber(number[3], wificolor);
}

void connectToNetwork() {
  Serial.println("");
  Serial.println("Connecting to SSID: " + String(networkSSID));

  WiFi.disconnect(true);
  WiFi.onEvent(WiFiEvent);

  WiFi.mode(WIFI_STA); //station
  WiFi.setSleep(false);

  WiFi.begin(networkSSID, networkPass);
}



// --------------------------------------------------------------------------------------------------------------------
// Setup is the pre-loop running program

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Initializing M5-Atom.");

  M5.begin(true, false, true);
  delay(50);
  M5.dis.drawpix(0, 0xf00000);

  // blanks out the screen
  // drawNumber(number[17], alloffcolor);
  wifiErrorSquare();
  delay(100); //wait 100ms before moving on

  connectToNetwork(); //starts Wifi connection
  delay(5000);
  while (!networkConnected) {
    connectToNetwork(); //starts Wifi connection
    delay(5000);
  }
  // UDP connect for OSC
  Udp.begin(localPort);
  // Flash screen if connected to wifi.
  drawNumber(number[0], alloffcolor);
  delay(100);
  drawNumber(number[2], wificolor);
  delay(400);
  drawNumber(number[0], alloffcolor);
  delay(200);
  drawNumber(number[2], wificolor);
  delay(400);
  drawNumber(number[0], alloffcolor);
  delay(200);
  drawNumber(number[2], wificolor);
  delay(400);
  drawNumber(number[0], alloffcolor);
  delay(100);
  redsquare();
}

void muteON(OSCMessage &rxmsg) {
  greensquare();
  micMuted = true; 
  Serial.println("Zoom mic muted!");
}

void muteOFF(OSCMessage &rxmsg) {
  redsquare();
  micMuted = false; 
  Serial.println("Zoom mic live!");
}


// --------------------------------------------------------------------------------------------------------------------
// This is the main program loop

void loop()
{
  OSCMessage msg;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/zoomosc/me/unMute", muteOFF);
      msg.dispatch("/zoomosc/me/mute", muteON);
      delay (100);
    } else {
      error = msg.getError();
      Serial.println("error: ");
      Serial.println(error);
    }
  }

  if (!networkConnected){
    // Lost Network Connection
    wifiErrorSquare();
    connectToNetwork(); //starts Wifi connection
    while (!networkConnected) {
      delay(200);
    }
     // UDP connect for OSC
     Udp.begin(localPort);
     if (micMuted){
      greensquare();
     } else {
      redsquare();
     }
   }
  
  delay(50);
  M5.update();
}

// --------------------------------------------------------------------------------------------------------------------
