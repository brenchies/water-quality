
#include <Wire.h>                //enable I2C.
#define addressT 102              //default I2C ID number for EZO RTD Circuit.
#define addressPH 99               //default I2C ID number for EZO pH Circuit.
#define addressDO 97               //default I2C ID number for EZO DO Circuit.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

float T;
float PH;
float DO;
char T_cal[4];
char *do_data;                        //char pointer used in string parsing.
char *sat_data;                       //char pointer used in string parsing.


// this int will hold the current count for our sketch
int count = 0;



// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("counter");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(count);
  counter->save(count);

  // increment the count by 1
  count++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}
