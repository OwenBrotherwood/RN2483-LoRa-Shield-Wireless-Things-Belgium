/*
 * Author: Lorenz Adriaensen
 * Date: 2017-01-30
 *
 * A library for using the Wireless 868 MHz LoRa Shield for Arduino UNO.
 *
 *
 * The RN2383 is connected as follows:
 * Reset -- 4
 * Uart TX -- 7
 * Uart RX -- 8
 *
 */

#include <SoftwareSerial.h>
#include <stdlib.h>

//Define Serial 
#define debugSerial Serial

//Define RN2483 Pins
#define resetpin 4
#define RN2483_tx_pin 7
#define RN2483_rx_pin 8

#define onewire_pin 2

long interval = 2; //interval of the loop time in minutes


//init sw serial for comm. with rn2483
SoftwareSerial loraSerial(RN2483_rx_pin, RN2483_tx_pin);

void setup()
{
  //start serial comm.
  debugSerial.begin(57600);
  loraSerial.begin(57600);

  RN2483_reset();
  RN2483_init();
}

void loop()
{
  const unsigned long minutes = interval * 60 * 1000UL;
  static unsigned long lastSampleTime = 0 - minutes;
  
  unsigned long now = millis();
  if (now - lastSampleTime >= minutes)
  {
    lastSampleTime += minutes;
    RN2483_send("DE10DE1976");
  }
}

void RN2483_reset()
{
  pinMode(resetpin,OUTPUT);
  delay(2000);
  digitalWrite(resetpin,HIGH);
  delay(1000);
  digitalWrite(resetpin,LOW);
  delay(200);
  debugSerial.write("RESTARTING RN2483\r\n");
  digitalWrite(resetpin,HIGH);
  delay(1000);
}

void RN2483_send(String data)
{
  //Send data
  debugSerial.println("mac tx uncnf 1 " + data);
  loraSerial.println("mac tx uncnf 1 " + data);
  RN2483_print_feedback();
  delay(2000);
}

void RN2483_init()
{
  //Configure this device to use activation by personalization.
  debugSerial.println();
  debugSerial.println("> mac join abp");
  loraSerial.println("mac join abp");
  RN2483_print_feedback();
  delay(2000);
  
  //Set the data rate
  debugSerial.println("> mac set dr 0");
  loraSerial.println("mac set dr 0");
  RN2483_print_feedback();
  delay(2000);
  debugSerial.println("> mac save");
  loraSerial.println("mac save");
  RN2483_print_feedback();
  delay(2000);
}

void RN2483_print_feedback()
{
  while (loraSerial.available()) 
  {
    debugSerial.print((char)loraSerial.read());
  }
  debugSerial.println();
}

