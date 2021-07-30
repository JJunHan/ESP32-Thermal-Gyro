#include "SPIFFS.h"
#include <Arduino.h>

String readFile(fs::FS &fs, const char * path){

  Serial.printf("Reading file: %s\r\n", path);
  if(!SPIFFS.begin(true)){
    Serial.println("Error mounting SPIFFS");
    return String();
  }
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  //Serial.println(fileContent);
  file.close();
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }

  int bytesWritten = file.print(message);

  if(bytesWritten > 0){ // Not empty
    Serial.println("- file written");
    Serial.printf("Bytes Written: %d \n" , bytesWritten);
  } 
  else {
    Serial.println("- write failed");
  }
  file.close();
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    if(!SPIFFS.begin(true)){
      Serial.println("Error mounting SPIFFS");
      return;
    }

    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
