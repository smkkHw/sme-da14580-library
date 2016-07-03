/*
SmeIoT Library - FwUpgrade

This Sketch update the Telit Chip to the GP.S03.02.00-B002GP.B00.01.0C version.
At the end the Telit Chip is upgraded to the newest version and its internal Baudrate
change to 115200

created 27 Nov 2015
by Mik (smkk@axelelettronica.it)

This example is in the public domain
https://github.com/ameltech

Telit le51-868-s more information available here:
http://www.telit.com/products/product-service-selector/product-service-selector/show/product/le51-868-s/
*/

#include <Arduino.h>
#include <Da14580.h>

#define W_1_MIN 60000 
#define W_10_SEC 10000 

int fwSize=-1;
FwDownloadEnu fwStatus=Idle;
bool fwDwnOK=false;
bool ledGLigth = false;
bool ledBLigth = false;

typedef enum {
    Idle,
    SizeWaiting,
    Downloading,
    Completed,
    SmartSnippet
}FwDownloadEnu;


static void ledError(bool error){
    if (error) {
        ledRedLight(LOW);
        ledGreenLight(HIGH);
        } else {
        ledRedLight(HIGH);
        ledGreenLight(LOW);
    }
}

static void smartSnippetsDownload(void) {
    SerialUSB.println("\n\n\nTo download the new Fw the SmartEverything and the SmarSnippets need to be in sync, follow the below steps to ensure the two component are in sync, after your answer of the question");
    SerialUSB.println("1) Disconnect this serial console and connect the SmartSnippets");
    SerialUSB.print("2) You have 1 minute of time to Browse the new Fw image ");
    SerialUSB.println("on the \"Booter\" tab of the SmartSnippets.");
    SerialUSB.println("3) Wait the GREEN LED on the SmartEverything to light on, and press \"Download\" within the next 10 Seconds.\n");
    SerialUSB.println("Are You ready [Y/N]?");
    char toolSelected =0;
    do {
        if (SerialUSB.available()) {
            toolSelected = (char)SerialUSB.read();
        }
    } while ((toolSelected!='Y') && (toolSelected !='y'));
    
    ledRedLight(HIGH);
    delay(W_1_MIN);
    ledRedLight(LOW);
    ledGreenLight(HIGH);
    delay(W_10_SEC);
    // send the BLE reset by moving its pin
}


// the setup function runs once when you press reset or power the board
void setup() {

    SerialUSB.begin(57600);
    // The secondary boot-loader sets the BLE Internal baud rate to 57600
    Serial1.begin(57600);

    // wait for serial port to connect.
    // Needed for native USB
    while (!SerialUSB) {
        ;
    }
    char toolSelected = 0;
    SerialUSB.println("Which FwDownload tool are You using ?");
    SerialUSB.println("1) Dialog SmartSnippet (sponsored)");
    SerialUSB.println("2) Bin to Serial stream of bytes");
    do {
        if (SerialUSB.available()) {
            toolSelected = (char)SerialUSB.read();
        }
    } while ((toolSelected!='1') && (toolSelected !='2'));
    
    if (toolSelected == '1') {
        smartSnippetsDownload();
        fwStatus = SmartSnippet;
    } else {
        SerialUSB.println("How big is the new Fw to Download in bytes ?  (end with CR/LF)");
        fwStatus = SizeWaiting;
    }
}

// the loop function runs over and over again forever
void loop() {
    switch (fwStatus) {
        
        case SmartSnippet:
        char readData;
        while (SerialUSB.available()) {
            readData = SerialUSB.read();
            Serial1.write(readData);
            ledGreenLight(ledGLigth);
            ledGLigth = !ledGLigth;
        }

        while (Serial1.available()) {
            readData = Serial1.read();
            SerialUSB.write(readData);
            ledBlueLight(ledBLigth);
            ledBLigth = !ledBLigth;
        }
        break;
        
        case Idle:
        break;
        
        case SizeWaiting:
        if (SerialUSB.available()) {
            ledGreenLight(HIGH);
            char inChar = (char)SerialUSB.read();
            if (fwSize==-1) // first byte received
            fwSize = inChar<<8;
            else
            fwSize |= inChar;
            
            // Received the Last byte LINE_FEED
            if (inChar == 0x0A) {
                SerialUSB.print("Waiting Da15480 reset to starting the download of ");
                SerialUSB.print(fwSize, DEC);
                SerialUSB.println(" Bytes ");
                fwStatus = Downloading;
            }
        }
        break;
        
        case Downloading:
        if (asmeBle.fwDownload(fwSize)) {
            SerialUSB.println("Fw Download completed");
            fwDwnOK = true;
            } else {
            SerialUSB.println("Error in Fw Download");
            fwDwnOK = false;
        }
        fwStatus = Completed;
        break;
        
        default:
        Completed:
        ledError(fwDwnOK);
        break;
    }
    
}


