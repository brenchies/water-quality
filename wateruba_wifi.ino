#include "config.h"
#include <Wire.h> 

#define addressT 102              //default I2C ID number for EZO RTD Circuit.
#define addressPH 99               //default I2C ID number for EZO pH Circuit.
#define addressDO 97               //default I2C ID number for EZO DO Circuit.

byte code = 0;                   //used to hold the I2C response code.
char I2C_data[20];               //we make a 20 byte character array to hold incoming data from the I2C circuit.
byte in_char = 0;                //used as a 1 byte buffer to store in bound bytes from the I2C Circuit.
byte i = 0;                      //counter used for I2C_data array.
int time_ = 815;                 //used to change the delay needed depending on the command sent to the EZO Class I2C Circuit. (600 for RTD, 815 for PH, 575 for DO)
float data_float;                //float var used to hold the float value of the I2C.
char T_cal[4];                   //variable used to calibrate DO sensor based on temperature
char *do_data;                   //char pointer used in string parsing.
char *sat_data;                  //char pointer used in string parsing.

// variables to hold our data
float T;
float PH;
float DO;

// set up the Adafruit IO feeds
AdafruitIO_Feed *W1temperature = io.feed("temperature");
AdafruitIO_Feed *W1acidity = io.feed("acidity");
AdafruitIO_Feed *W1oxygen = io.feed("oxygen");

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

  // hopefully we are connected
  Serial.println();
  Serial.println(io.statusText());

  //calibrating the DO sensor for temperature
  T = i2cRead(addressT);                    
  sprintf(T_cal, "%.2f", T);                   
  Wire.beginTransmission(addressDO);        
  Wire.write("T,");
  Wire.write(T_cal);               
  Serial.print("T,");
  Serial.println(T_cal);
  Wire.endTransmission();                   
  delay(time_);
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

  delay(period) ;
  
  // read all three sensors
  T = i2cRead(addressT);      //temperature
  PH = i2cRead(addressPH);    //acidity
  DO = i2cRead(addressDO);    //dissolved oxygen

  // send measured values to Adafruit IO
  temperature->save(T);
  acidity->save(PH);
  oxygen->save(DO);

  // send data to serial for troubleshooting offline monitoring
  Serial.print("T = ");
  Serial.println(T);
  Serial.print("PH = ");
  Serial.println(PH);
  Serial.print("DO = ");
  Serial.print(DO);
}


// function for reading data from sensors through I2C, works with all three sensors
float i2cRead(int address) {
        
    Wire.beginTransmission(address);           //call the circuit by its ID number.
    Wire.write('r');                       //the command to read data from I2C Atlas sensors.
    Wire.endTransmission();                   //end the I2C data transmission.
    delay(time_);                            //wait the correct amount of time for the circuit to complete its instruction.
    Wire.requestFrom(address, 20, 1);       //call the circuit and request 20 bytes (this may be more than we need)
    code = Wire.read();                    //the first byte is the response code, we read this separately.
    while (Wire.available()) {            //are there bytes to receive.
      in_char = Wire.read();              //receive a byte.
      I2C_data[i] = in_char;              //load this byte into our array.
      i += 1;                             //incur the counter for the array element.
      if (in_char == 0) {                 //if we see that we have been sent a null command.
        i = 0;                            //reset the counter i to 0.
        break;                            //exit the while loop.
      }
    }
    if (address==addressDO) {
      do_data = strtok(I2C_data, ",");
      sat_data = strtok(NULL, ",");
      data_float = atof(do_data);
    } else {      
      data_float = atof(I2C_data);
    }
    return data_float;
}
