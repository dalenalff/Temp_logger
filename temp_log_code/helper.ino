/***********************************************************
 Author: Markus Ritschel
 eMail:  git@markusritschel.de
 Date:   2023-03-01

 This module holds a bunch of helper  functions for 
 - timestamp creation
 - two-digit string formatting
 - writing simultaneously to SD card and to terminal output
************************************************************/


String getISOtime() {
    /* Create a timestamp of the ISO8601 format */
    DateTime now = rtc.now();
    String ISO8601_time = "";

    ISO8601_time +=  now.year();
    ISO8601_time +=  "-";
    ISO8601_time +=  twodigits(now.month());
    ISO8601_time +=  "-";
    ISO8601_time +=  twodigits(now.day());
    ISO8601_time +=  "T";
    ISO8601_time +=  twodigits(now.hour());
    ISO8601_time +=  ":";
    ISO8601_time +=  twodigits(now.minute());
    ISO8601_time +=  ":";
    ISO8601_time +=  twodigits(now.second());
    ISO8601_time +=  "Z";

    return ISO8601_time;
}


String twodigits(int x) {
    /* Helper function to format datetime elements as two digit (leading zeros) string */
    String output = "";
    if (x < 10) {
        output += "0";
        output += x;
    } else {
        output = x;
    }
    return output;
}


void printOutput(String dataString) {
    /* Helper function to write a string to both the Serial output and the SD card */
    File dataFile = SD.open(logfile, FILE_WRITE);
    // if the file is available, write to it and close it again
    if (dataFile) {
        dataFile.print(dataString);
        dataFile.close();
    };
    Serial.print(dataString);
}


void printOutputln(String dataString) {
    /* Helper function to write a line to both the Serial output and the SD card */
    File dataFile = SD.open(logfile, FILE_WRITE);
    // if the file is available, write to it and close it again
    if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
    };
    Serial.println(dataString);
}
