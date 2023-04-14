#include <DallasTemperature.h> // Library for DallasTemperature sensor
#include <RTClib.h> // Library for real-time clock (RTC)
#include <OneWire.h> // Library for one-wire communication
#include <SPI.h> // Library for SPI communication
#include <SD.h> // Library for SD card communication
#include "ds_logger_demo.h" // Library to set-up the temperature logger as described in the DS18B20 manual

// C++ code
//

const String logfile = "tsensor.log"; //name of the file output

RTC_DS1307 rtc; // Real - time clock object
OneWire ow(4); // OneWire communication object

void setup() {
  
  Serial.begin(9600); // Initialize serial communication

  // Initialize RTC
  if(!rtc.begin()) { // Check if RTC is running
    Serial.println("RTC is NOT running. Lets set the time now");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  }

  // Initialize SD card
  if(!SD.begin(10)) { // Check if SD module properly works 
    Serial.println("SD module initialization failed or Card is not present");
    return;
  }
}

// we dont have a parity check in our code
void loop() {
  
  byte rom_code[8]; // creates an array containing 8 elements of type byte for the rom
  byte sp_data[9]; //creates an array of 9 bytes. It will contain data from scratchpad
  
  // Oth sequence: (reset, then read)
  // Start sequence to read out the rom code (sensor family on LSB, then 48-bit registration number)
  ow.reset();
  ow.write(READ_ROM);
  // rom _code reads from least significant bit, to most significant bit
  for (int i=0; i<8; i++){
    rom_code[i] = ow.read(); 
  }
  
  // 0x28 is the first bite LSB in rom_code 
  // Check if the sensor is a DS18B20 sensor based on the ROM code
  if(rom_code[0] != IS_DS18B20_SENSOR){
    Serial.println("Sensor is not a DS18B20 sensor!");
  }
  
  //Concatenate the information of 6 bits from the rom_code in hexadecimal format
  String registration_number;
  for (int i=1; i<7; i++){
    registration_number += String(rom_code[i], HEX);
  }

  // Start the sequence to convert temperature
  ow.reset();
  ow.write(SKIP_ROM);
  ow.write(CONVERT_T);
  
  // Start the sequence for reading data from scratchpad
  ow.reset();
  ow.write(SKIP_ROM);
  ow.write(READ_SCRATCHPAD);

  // Read 9 bytes of data from scratchpad, but we are just interested in the first 2 bytes (16 bits) TEMPERATURE

  for (int i=0; i<9; i++){
    sp_data[i] = ow.read();    
  }

  // 8-bits shift. byte 1 comes first than byte 0 in an array
  int16_t tempRead = sp_data[1] << 8 | sp_data[0];

  //Convert to Temperature [Celcius] from hexadecimal to decimal
  float tempCelcius = tempRead / 16.0; // divide by 2¨**4 =16 for four digits after the comma
  
  // Print temperature data to serial monitor
  Serial.println(tempCelcius);
  
  // Store data in SD card in the format:  ## time; millis; registration number; Temp
  printOutput(getISOtime());
  printOutput("; ");
  printOutput((String) millis());
  printOutput("; ");
  printOutput(registration_number);
  printOutput("; ");
  printOutputln((String)tempCelcius);
  
  delay(1000);
  }
