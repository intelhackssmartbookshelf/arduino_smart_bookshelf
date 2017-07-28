/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include <CurieBLE.h>
#include <NewPing.h>



#define TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define TRIGGER_PI  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PI    6  // Arduino pin tied to echo pin on the ultrasonic sensor.

NewPing bonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PI, ECHO_PI, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


int latchPin = 10;      //Pin connected to ST_CP of 74HC595
int clockPin = 8;   //Pin connected to SH_CP of 74HC595
int dataPin = 9;    //Pin connected to DS of 74HC595

byte led = 0;

const int ledPin = 13; // set ledPin to use on-board LED

BLEService ledService("94:76:B7:D5:F1:8D"); // create service

// create switch characteristic and allow remote device to read and write
BLECharCharacteristic switchChar("94:76:B7:D5:F1:8D", BLERead | BLEWrite);
BLECharCharacteristic ledc("94:76:B7:D5:F1:8D", BLERead | BLEWrite);
BLECharCharacteristic dis1("94:76:B7:D5:F1:8D", BLERead | BLENotify);
BLECharCharacteristic dis2("94:76:B7:D5:F1:8D", BLERead | BLENotify);



void setup() {
  Serial.begin(9600);
    updateShiftRegister();
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output

  pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  // begin initialization
  BLE.begin();

  // set the local name peripheral advertises
  BLE.setLocalName("LEDCB");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchChar);
   ledService.addCharacteristic(ledc);
  ledService.addCharacteristic(dis1);
  ledService.addCharacteristic(dis2);

  // add service
  BLE.addService(ledService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchChar.setEventHandler(BLEWritten, switchCharacteristicWritten);
  ledc.setEventHandler(BLEWritten, ledcWritten);
  // set an initial value for the characteristic
  switchChar.setValue(0);
  ledc.setValue(0);
  dis1.setValue(bonar.ping_cm());
  dis2.setValue(sonar.ping_cm());
  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loop() {
  // poll for BLE events
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");

  if (switchChar.value()) {
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }
}

  
void ledcWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
updateShiftRegister();

  bookin();
 
 
}


void updateShiftRegister() {
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, led);
   digitalWrite(latchPin, HIGH);
}

void bookin() {
   if (ledc.value()==0) {
           bitSet(led, 0);
      updateShiftRegister();
  } else if(ledc.value()==1) {
     bitSet(led, 1);
      updateShiftRegister();
  }
      else if(ledc.value()==2) {
     bitSet(led, 2);
      updateShiftRegister();
  } else if(ledc.value()==3) {
     bitSet(led, 3);
      updateShiftRegister();
  }
      else if(ledc.value()==4) {
     bitSet(led, 4);
      updateShiftRegister();
  }
      else if(ledc.value()==5) {
     bitSet(led, 5);
      updateShiftRegister();
  }
      else if(ledc.value()==6) {
     bitSet(led, 6);
      updateShiftRegister();
  }
      else if(ledc.value()==7) {
     bitSet(led, 7);
      updateShiftRegister();
  }
  
 

}

void bookout() {
        if(i<=5&&j<=5){
      bitClear(led, 0);
      updateShiftRegister();
      }
        else if(j<=10&&i<=10){
      bitClear(led, 1);
      updateShiftRegister();
        }
        else if(j<=15&&i<=15){
         bitClear(led, 2);
      updateShiftRegister();
        }
        else if(j<=20&&i<=20){
         bitClear(led, 3);
      updateShiftRegister();
        }
        else if(j<=25&&i<=25){
         bitClear(led, 4);
      updateShiftRegister();
        }
        else if(j<=30&&i<=30){
         bitClear(led, 5);
      updateShiftRegister();
        }
        else if(j<=35&&i<=35){
         bitClear(led, 6);
      updateShiftRegister();
      }
      else if(j<=40&&i<=40){
       bitClear(led, 7);
      updateShiftRegister();
      }
}


/*
  Copyright (c) 2016 Intel Corporation. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-
  1301 USA
*/
