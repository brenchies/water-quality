#include "config.h"
#include <Wire.h>                //enable I2C.
#define addressT 102              //default I2C ID number for EZO RTD Circuit.
#define addressPH 99               //default I2C ID number for EZO pH Circuit.
#define addressDO 97               //default I2C ID number for EZO DO Circuit.

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

int count = 0;

int period = 10000; //delay between samples

// need to add feeds for temperature, DO, and pH <<<<<<<<<<<<<<<<<<<<<<<<<<
AdafruitIO_Feed *counter = io.feed("counter");

void setup() {
  Serial.begin(115200);
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
     
  } 

  //reading temperature in order to calibrate DO sensor
  T = i2cRead(addressT);
  sprintf(T_cal, "%.2f", T);
  Wire.beginTransmission(addressDO);           //call the circuit by its ID number.
  Wire.write("T,");
  Wire.write(T_cal);                       //the command to read data from I2C Atlas sensors.
  Serial.println();
  Serial.println(io.statusText());
  Wire.endTransmission();                   //end the I2C data transmission.
  delay(time_);                            //wait the correct amount of time for the circuit to complete its instruction.
}

void loop() {

  io.run();
  
  Serial.print("sending -> ");
  Serial.println(count);
  counter->save(count);
  count++;

  //need to also add lines to save the below parameters to the Adafruit IO feeds <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  delay(period);
  T = i2cRead(addressT);
  PH = i2cRead(addressPH);
  DO = i2cRead(addressDO);
  Serial.print(T);
  Serial.print(",");
  Serial.print(PH);
  Serial.print(",");
  Serial.println(DO);


}


//function for reading sensors through i2c given their address, outputs data reading
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
