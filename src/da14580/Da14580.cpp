/*
* Da14580.cpp
*
* Created: 28/2/2016 10:15:50 PM
* by Mik (smkk@axelelettronica.com)
*
*/
#include <Arduino.h>
#include "../Da14580.h"


Da14580::Da14580(void) {
    
}

void Da14580::begin (unsigned long baudRate){
    Serial1.begin(baudRate);
}

/*
*
*     DA14580/581 (BLE)     Data direction          External device
* "0x02 (Start TX, STX)"        >
*                               <            "0x01 (Start of Header, SOH)"
*                               <                       LEN_LSB
*                               <                       LEN_MSB
* 0x06 (ACK) or 0x15 (NACK)	    >
* Copy the data to System RAM	<          #bytes sent: N = LEN_MSB * 8 + LEN_LSB
* Calculate & Send the CRC 	    >                 Read/Check the CRC
* Branch to 0x20000000 & Flash  <               0x06 (ACK) or 0x15 (NACK)
*
*/
bool Da14580::fwDownload(int size) {
    long timeOut, startTime=millis();
    char inChar, crc=0,  sizeB[2];
    bool continueDwn = false;
    
    
    // wait for Start TX
    do {
        // get the new byte:
        if (Serial1.available()) {
            inChar = (char)Serial1.read();
            
            if (inChar == (char)STX) {
                continueDwn = true;
            }
        }
        
        // exit with error if Timeout
        delay(5);
        timeOut = (millis()-startTime);
        if (ACK_TIMEOUT_20SEC < timeOut)
        return false;
    } while (!continueDwn);
    
    // Send Header
    sizeB[0] = SOH;
    Serial1.write(&sizeB[0], 1);
    
    sizeB[1] = size>>8;
    sizeB[0] = (char)size;
    Serial1.write(&sizeB[0], 1);
    Serial1.write(&sizeB[1], 1);
    
    // wait for an ACK;
    startTime = millis();
    continueDwn = false;
    do {
        // get the new byte:
        if (Serial1.available()) {
            inChar = (char)Serial1.read();
            
            if (inChar == (char)ACK) {
                continueDwn = true;
            }
        }
        
        // exit with error if Timeout
        delay(5);
        timeOut = (millis()-startTime);
        if (DATA_TIMEOUT_2SEC < timeOut)
        return false;
    }while (!continueDwn);
    
    delay(50);
    //Start Real downloading
    bool downloadFinish = false;
    startTime = millis();
    do {
        // get the new byte:
        if (SerialUSB.available()) {
            inChar = (char)SerialUSB.read();
            
            Serial1.write(&inChar, 1);
            crc ^= inChar;          // calculate CRC
            size--;
            
            startTime = millis();   // reset Timeout
        }
        
        if (size == 0) // ent of Fw donwload
        downloadFinish = true;
        timeOut = (millis()-startTime);
        if (DWNLD_TIMEOUT_5MIN < timeOut) {
            inChar = NAK;
            Serial1.write(&inChar, 1);
            return false;
        }
    }while (!downloadFinish);

    delay(50);
    // Wait the CRC from DA15480
    startTime = millis();
    continueDwn = false;
    do {
        // get the new byte:
        if (Serial1.available()) {
            inChar = (char)Serial1.read();
            
            if (inChar == crc) {
                inChar = ACK;
                Serial1.write(&inChar, 1);
                continueDwn = false;
            }
        }
        
        // exit with error if Timeout
        delay(5);
        timeOut = (millis()-startTime);
        if (DATA_TIMEOUT_2SEC < timeOut) {
            inChar = NAK;
            Serial1.write(&inChar, 1);
            return false;
        }
        
    }while (!continueDwn);

    return true;            
}


Da14580  asmeBle;
