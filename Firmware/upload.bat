echo off
REM A simple batch file to upload the sketch to my arduino compatible board using WinAVR
REM build of avrdude
REM
REM by Peter Dunshee
REM
avrdude -b 57600 -p atmega328p -c arduino -P COM3 -U flash:w:AvrKeyboardToy.hex
