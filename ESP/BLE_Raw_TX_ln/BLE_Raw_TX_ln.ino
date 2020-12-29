//#include "Arduino.h"
#include <FS.h>
//#include "Wav.h"
#include "I2S.h"
//#include <SD.h>
//#include<Wire.h>
//#include<SPI.h>
#include "BluetoothSerial.h"


//comment the first line and uncomment the second if you use MAX9814
//#define I2S_MODE I2S_MODE_RX
#define I2S_MODE I2S_MODE_ADC_BUILT_IN

#define I2S_PORT I2S_NUM_0 // change in the library if changed here

BluetoothSerial SerialBT;
TaskHandle_t taskHandler = NULL;

//const static int val_shift = 13;
//const static int val_offset = 14000;

const char filename[] = "/sound.wav";

const int record_time = 1*60;  // second
const int headerSize = 44;     //Wave Header
const int waveDataSize = record_time * 75680; //88000; // (44100 hz, 16 bit and 1 channel)/8 75680
const int numCommunicationData = 8000;
const int numPartWavData = numCommunicationData / 4;
byte header[headerSize];
//char communicationData[numCommunicationData];
//char partWavData[numPartWavData];

char* communicationData;
char* partWavData;

//char line[] = "/n";





void setup() {
  Serial.begin(115200);
  setCpuFrequencyMhz(240);

  if (!SerialBT.begin("ESP32_Bare")) {
    Serial.println("An error Occured in Initialization of BLE");
    ESP.restart();
    return;
  } else {
    Serial.println("BLE initialized");
  }
  SerialBT.register_callback(btCallback);

  Serial.println("BLE Availabe for Pair");




}

void loop() {
}
