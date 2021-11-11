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

int period = 10000; //delay between samples

#include <WiFi.h>
#include <WiFiMulti.h>

// Identification
int unitid=24;
int count = 0;
const char* device = "omega";

// WiFi parameters
const char* ssid = "Your_Wifi"; //insert your wifi SSID here
const char* password = "Your_Password"; //insert your wifi password here
String thingName="data_collector.php?unitid=24"; //thing for grouping all the data together 
const char* host = "YourWebsite.com"; //host :) for tcp connection

// Data stuff
String arrayVariableNames[]={"variable","unitid","temp","ph","do"}; //THIS WAY WE REFER TO EACH VARIABLE AS AN INDEX IN THIS GLOBAL ARRAY.

float arrayVariableValues[5];
int numberVariables=sizeof(arrayVariableValues)/sizeof(arrayVariableValues[0]); //tells the number of arguments inside each array

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

  if (WiFi.status() == WL_CONNECTED) { 
    Serial.println(F("WiFi connected")); 
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP()); 
    Serial.println(F("======================================================"));  
  }
  
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


  //Reconnect the WIFI if it is out!!!!! <<<<<<<<<<<<<<<<<<<<<<<<<<<<
  if ((WiFi.status() != WL_CONNECTED)) { 
    WiFi.begin(ssid, password);
    while((WiFi.status() != WL_CONNECTED)) {
      delay(500); 
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) { 
      Serial.println(F("WiFi connected")); 
      Serial.println("IP address: "); 
      Serial.println(WiFi.localIP()); 
      Serial.println(F("======================================================"));
    }
     delay(3000);
  } 
  
  delay(period);
  T = i2cRead(addressT);
  PH = i2cRead(addressPH);
  DO = i2cRead(addressDO);
  Serial.print(T);
  Serial.print(",");
  Serial.print(PH);
  Serial.print(",");
  Serial.println(DO);
  arrayVariableValues[0]=400;  
  arrayVariableValues[1]=unitid;
  arrayVariableValues[2]=T;
  arrayVariableValues[3]=PH;
  arrayVariableValues[4]=DO;
  // send data to cws
  dweetdata();//upload it 
}

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


//connects TCP,sends dweet,drops connection, prints the server response on the console
void dweetdata(){
  // Use WiFiClient class to create TCP connections 
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) { 
    Serial.println("connection failed"); 
    return; 
  } 
  client.print(GetDweetStringHttpBuilder()); 
  //slow doown ...stability stuff
  delay(1000);
  // Read all the lines of the reply from dweet server and print them to Serial 
  while(client.available()){ 
    String line = client.readStringUntil('\r'); 
    //Serial.print(line); 
  } 
  //just display ending conection on the serial port 
  Serial.println(); 
  Serial.println("closing connection"); 
}

String GetDweetStringHttpBuilder() {
  int i=0; 
  String dweetHttpGet="GET /waterquality/"; //initial empty get request
  dweetHttpGet=dweetHttpGet+String(thingName)+"&"; //start concatenating the thing name (dweet.io)
  for(i=0;i<(numberVariables);i++){ //concatenate all the variable names and values
  if(i==numberVariables-1){ 
    dweetHttpGet=dweetHttpGet +  String(arrayVariableNames[i]) + "="+ String( arrayVariableValues[i]); //the lastone doesnt have a "&" at the end 
  }
  else 
    dweetHttpGet=dweetHttpGet + String(arrayVariableNames[i]) + "="+ String( arrayVariableValues[i]) + "&";   
  }
  dweetHttpGet=dweetHttpGet + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"; 
  return dweetHttpGet; //this is our freshly made http string request 
}
