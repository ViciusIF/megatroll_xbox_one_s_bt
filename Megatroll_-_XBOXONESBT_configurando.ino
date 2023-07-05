/*
 Example sketch for the Xbox One S Bluetooth library - developed by Kristian Sloth Lauszus
 For more information visit the Github repository: github.com/felis/USB_Host_Shield_2.0 or
 send me an e-mail:  lauszus@gmail.com
 */

//------------------------------ Modificação ------------------------------

 //definindo pinos do motor
#define pinMotF 5
#define pinMotB 6

//Incluindo biblioteca do servomotor com controle de velocidade
#include <Servo.h>
//definindo pino do servomotor
#define pinServo 3
// nomeando o servo
Servo servo_direcao;

//------------------------------ FIM ------------------------------

#include <XBOXONESBT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statement in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the XBOXONESBT class in two ways */
// This will start an inquiry and then pair with the Xbox One S controller - you only have to do this once
// You will need to hold down the Sync and Xbox button at the same time, the Xbox One S controller will then start to blink rapidly indicating that it is in pairing mode
XBOXONESBT Xbox(&Btd, PAIR);

// After that you can simply create the instance like so and then press the Xbox button on the device
//XBOXONESBT Xbox(&Btd);

void setup() {

  //------------------------------ Modificação ------------------------------
  
  //pinos digitais do motor
  pinMode(pinMotF, OUTPUT);
  pinMode(pinMotB, OUTPUT);
  
  //pino digital do servomotor
  servo_direcao.attach(pinServo);

  //------------------------------ FIM ------------------------------
  
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox One S Bluetooth Library Started"));  
}

void loop() {
  Usb.Task();

  if (Xbox.connected()) {

  //------------------------------ Modificação ------------------------------
    
    //Serial.print("\nConectado"); // print em monitor serial que o status do controle é "Conectado"

    //Direcional analógico esquerdo => extersão das rodas frontais
    servo_direcao.write(map(Xbox.getAnalogHat(LeftHatX), -32768, 32767, 0, 180));   // 85 => centralizado / 40 => esquerda / 130 => direita
    
    //Gatilho direito (RT) => aceleração da roda traseria
    analogWrite(pinMotF, map(Xbox.getButtonPress(RT), 0, 1023, 0, 255));

    //Gatilho esquerdo (LT) => freio e ré da roda traseria
    analogWrite(pinMotB, map(Xbox.getButtonPress(LT), 0, 1023, 0, 255));
    
  //------------------------------ FIM ------------------------------
    
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) { 
        Serial.print(F("LeftHatX: "));
        Serial.print(Xbox.getAnalogHat(LeftHatX));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("LeftHatY: "));
        Serial.print(Xbox.getAnalogHat(LeftHatY));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
        Serial.print(F("RightHatX: "));
        Serial.print(Xbox.getAnalogHat(RightHatX));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
        Serial.print(F("RightHatY: "));
        Serial.print(Xbox.getAnalogHat(RightHatY));
      }
      Serial.println();
    }

    if (Xbox.getButtonPress(LT) > 0 || Xbox.getButtonPress(RT) > 0) {
      if (Xbox.getButtonPress(LT) > 0) {
        Serial.print(F("LT: "));
        Serial.print(Xbox.getButtonPress(LT));
        Serial.print("\t");
      }
      if (Xbox.getButtonPress(RT) > 0) {
        Serial.print(F("RT: "));
        Serial.print(Xbox.getButtonPress(RT));
        Serial.print("\t");
      }
      Serial.println();
    }

//    // Set rumble effect
//    static uint16_t oldLTValue, oldRTValue;
//    if (Xbox.getButtonPress(LT) != oldLTValue || Xbox.getButtonPress(RT) != oldRTValue) {
//      oldLTValue = Xbox.getButtonPress(LT);
//      oldRTValue = Xbox.getButtonPress(RT);
//      uint8_t leftRumble = map(oldLTValue, 0, 1023, 0, 255); // Map the trigger values into a byte
//      uint8_t rightRumble = map(oldRTValue, 0, 1023, 0, 255);
//      if (leftRumble > 0 || rightRumble > 0)
//        Xbox.setRumbleOn(leftRumble, rightRumble, leftRumble, rightRumble);
//      else
//        Xbox.setRumbleOff();
//    }

    if (Xbox.getButtonClick(UP))
      Serial.println(F("Up"));
    if (Xbox.getButtonClick(DOWN))
      Serial.println(F("Down"));
    if (Xbox.getButtonClick(LEFT))
      Serial.println(F("Left"));
    if (Xbox.getButtonClick(RIGHT))
      Serial.println(F("Right"));

    if (Xbox.getButtonClick(VIEW))
      Serial.println(F("View"));
    if (Xbox.getButtonClick(MENU))
      Serial.println(F("Menu"));
    if (Xbox.getButtonClick(XBOX)) {
      Serial.println(F("Xbox"));
      Xbox.disconnect();
    }

    if (Xbox.getButtonClick(LB))
      Serial.println(F("LB"));
    if (Xbox.getButtonClick(RB))
      Serial.println(F("RB"));
    if (Xbox.getButtonClick(LT))
      Serial.println(F("LT"));
    if (Xbox.getButtonClick(RT))
      Serial.println(F("RT"));
    if (Xbox.getButtonClick(L3))
      Serial.println(F("L3"));
    if (Xbox.getButtonClick(R3))
      Serial.println(F("R3"));

    if (Xbox.getButtonClick(A))
      Serial.println(F("A"));
    if (Xbox.getButtonClick(B))
      Serial.println(F("B"));
    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));
    if (Xbox.getButtonClick(Y))
      Serial.println(F("Y"));
  }
}
