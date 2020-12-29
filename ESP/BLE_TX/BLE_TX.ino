//#include "Arduino.h"
#include <FS.h>
#include "Wav.h"
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


const char filename[] = "/sound.wav";

const int record_time = 10;  // second
const int headerSize = 44;
const int waveDataSize = record_time * 88000;
const int numCommunicationData = 8000;
const int numPartWavData = numCommunicationData / 4;
byte header[headerSize];
//char communicationData[numCommunicationData];
//char partWavData[numPartWavData];

char* communicationData;
char* partWavData;
char c[]="/n";
TaskHandle_t taskHandler = NULL;





void setup() {
  Serial.begin(115200);
  setCpuFrequencyMhz(240);

  if (!SerialBT.begin("ESP32")) {
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
