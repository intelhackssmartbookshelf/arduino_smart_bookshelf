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



const int ledPin = 13; // set ledPin to use on-board LED

BLEService ledService("94:76:B7:D5:F1:8D"); // create service

// create switch characteristic and allow remote device to read and write
BLECharCharacteristic dis1("94:76:B7:D5:F1:8D", BLERead | BLENotify);
BLECharCharacteristic dis2("94:76:B7:D5:F1:8D", BLERead | BLENotify);

long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output

  // begin initialization
  BLE.begin();

    // set the local name peripheral advertises
  BLE.setLocalName("LEDCB");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service

  ledService.addCharacteristic(dis1);
  ledService.addCharacteristic(dis2);
  
  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loop() {
  // poll for BLE events
  BLE.poll();

  // add service
  BLE.addService(ledService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);


  // set an initial value for the characteristic


  // start advertising
  BLE.advertise();

  long currentMillis = millis() ;
  if((currentMillis-previousMillis)>=200 ){
    previousMillis = currentMillis;
    if((bonar.ping_cm()!=0)&&(sonar.ping_cm()!=0))
    {
      dis1.setValue(bonar.ping_cm());
      dis2.setValue(sonar.ping_cm());
   }
  }
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







