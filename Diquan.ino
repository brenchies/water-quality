
/************************ Example Starts Here *******************************/
#define addressT 102              //default I2C ID number for EZO RTD Circuit.
#define addressPH 99               //default I2C ID number for EZO pH Circuit.
#define addressDO 97               //default I2C ID number for EZO DO Circuit.
#include "config.h"
#include <Wire.h> 

byte code = 0;                   //used to hold the I2C response code.
char I2C_data[20];               //we make a 20 byte character array to hold incoming data from the I2C circuit.
byte in_char = 0;                //used as a 1 byte buffer to store in bound bytes from the I2C Circuit.
byte i = 0;                      //counter used for I2C_data array.
int time_ = 815;                 //used to change the delay needed depending on the command sent to the EZO Class I2C Circuit. (600 for RTD, 815 for PH, 575 for DO)
float data_float;                //float var used to hold the float value of the I2C.
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

int period = 10000; //delay between samples

void setup() {

  // start the serial connection
  Serial.begin(115200);
  Wire.begin();                 //enable I2C port
 
  

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

    if (WiFi.status() == WL_CONNECTED) { 
    Serial.println(F("WiFi connected")); 
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP()); 
    Serial.println(F("======================================================"));  
  }

                         //wait the correct amount of time for the circuit to complete its instruction.



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
