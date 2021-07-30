# ESP32 Thermal Gyro

Mini project that i designed on EASYEDA consisting of an ESP32 with thermal and gyro sensors on a 2 layer PCB

## Features

- Gyroscope, Accelerometer (MPU-6050_C24112)
- Humidity, Temperature (HDC1080DMBR)
- USB to Serial (CH340C)
- Microcontroller (ESP32-WROVER-E)
- LED Driver (LED1642GWPTR)
- Buzzer (HNB09A05)
- 4 display 7 segment (FJ5461BH-red-orange)
- Serveral LEDs to display the functionality of the board and microC

## Takeaways

- Surface mount soldering (SMT)
- Basic circuit designing
- Basic PCB routing & tracing
- GPIO interactions
- Managing serial communication protocols

## Work in progress

- Unable to control LED driver

## How it looks like

![Sample](source/sample.png)

## Things to take note

- Watch out when choosing a Mosfet to drive a component. Make sure that it saturates upon the correct GATE-SOURCE VOLTAGE (V)
- Read the data sheet ensure it is an IO port before connecting to I2C bus... (port 34,35 are NOT I2C bus compatible)
- Check the schematic and ensure all pins go to where they are suppose to go
- HDC1080 has different configuration from HDC2010 and 2080 hence their software kits wont work. Also, you must edit the SDA SCL pins yourself Via Wire.h
