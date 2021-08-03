#ifndef FLIES_H
#define FLIES_H

#include "SPIFFS.h"
#include <Arduino.h>

String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);



#endif