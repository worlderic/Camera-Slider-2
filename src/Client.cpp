#include <Arduino.h>
#include <HardwareSerial.h>
#include "Client.h"
#include "Helpers.h"
#include "Constants.h"

Client::Client(Stream & s) : serial(s) {}

void Client::sendMessage(const unsigned char command,
                         const unsigned char * payload,
                         const unsigned int payloadLength) {

  unsigned int size = sizeof(char) * (payloadLength + 2);
  unsigned char msg[size];

  msg[0] = Constants::FLAG_START;
  msg[1] = command;

  // TODO Allow messages without payload
  memcpy(msg + 2, payload, payloadLength);
  memcpy(msg + payloadLength + 1, &Constants::FLAG_STOP, 1);

  serial.write(msg, sizeof(msg));
}

void Client::sendHandshakeGreetingMessage(void) {
  unsigned char data[17];
  memcpy(data, Constants::WELCOME_MESSAGE,
         strlen(Constants::WELCOME_MESSAGE));
  sendMessage(Commands::SEND_HANDSHAKE_GREETING, data, sizeof(data));
}

void Client::sendStatus(void) {
  unsigned char data[20];
  sendMessage(Commands::SEND_STATUS, data, sizeof(data));
}

void Client::sendPosition(void) {
  unsigned char data[20];

  // Convert position integers into char arrays
  unsigned char slide[4], pan[4], tilt[4], focus[4], zoom[4];
  intToByteArray(slide, 52300);
  intToByteArray(pan, 7250);
  intToByteArray(tilt, -2000);
  intToByteArray(focus, 0);
  intToByteArray(zoom, 5671);

  // Copy individual byte sets into data
  memcpy(data, slide, 4);
  memcpy(data + 4, pan, 4);
  memcpy(data + 8, tilt, 4);
  memcpy(data + 12, focus, 4);
  memcpy(data + 16, zoom, 4);

  sendMessage(Commands::SEND_POSITION, data, sizeof(data));
}

void Client::notifyHomingDone(void) {
  unsigned char dumbData[1] = { 0x22 };
  sendMessage(Commands::HOMING_DONE, dumbData, sizeof(dumbData));
}

void Client::notifyDataDownloadFinished(void) {
  unsigned char dumbData[1] = { 0x22 };
  sendMessage(Commands::DATA_RECEIVED, dumbData, sizeof(dumbData));
}
