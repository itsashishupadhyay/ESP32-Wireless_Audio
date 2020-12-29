//****************************************************BLE****************************************************************************************
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
  } else if (event == ESP_SPP_DATA_IND_EVT) {
    Serial.printf("ESP_SPP_DATA_IND_EVT len=%d handle=%d \n", param->data_ind.len, param->data_ind.handle);
    int dataLen = param->data_ind.len + 1;
    char textArray[dataLen];
    strncpy(textArray, (const char*)param->data_ind.data, dataLen);
    textArray[dataLen - 1] = 0;
    String textString = "";
    for (int i = 0; i <= dataLen - 4; i++) {
      Serial.println(textArray[i], HEX);
      textString += textArray[i];
    }
    //    String textString = textArray;
    Serial.printf("*****Text String:%s\n", textString);

    if (textString.equalsIgnoreCase ("START")) {
      Serial.println("Start Condition True");
      startRecording();
    } else if (textString.equals ("STOP")) {
      finishRecording();
    } else {
      Serial.println("Cant understand");
      Serial.println(textString);
    }
  }
}
//**********************************************Star/Stop Recording**************************************************************************
void startRecording() {

  I2S_Init(I2S_MODE, I2S_BITS_PER_SAMPLE_32BIT);
  i2s_start(I2S_PORT);
  Serial.println("***********Start Recording***********");
  communicationData = (char*) calloc(numCommunicationData, sizeof(char));
  partWavData = (char*) calloc(numPartWavData, sizeof(char));
  /* Disabled as i Want Raw DATA in to the buffer
  //    communicationData = (uint32_t*) calloc(numCommunicationData, sizeof(uint32_t));
  //    partWavData = (uint16_t*) calloc(numPartWavData, sizeof(uint16_t));

  //  CreateWavHeader(header, waveDataSize);
  //  SerialBT.write(header, headerSize);
*/
  xTaskCreate(i2s_adc, "i2s_adc", 1024 * 2, NULL, 1, &taskHandler);
}

void finishRecording() {
  Serial.println("***********End of Recording***********");
  xTaskCreate(i2s_cancel, "i2s_cancel", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

//**********************************************DataScale***********************************************************************************
//void i2s_adc_data_scale(uint8_t * d_buff, uint8_t* s_buff, uint32_t len)
//{
//  uint32_t j = 0;
//  uint32_t dac_value = 0;
//  for (int i = 0; i < len; i += 2) {
//    dac_value = ((((uint16_t) (s_buff[i + 1] & 0xf) << 8) | ((s_buff[i + 0]))));
//    d_buff[j++] = 0;
//    d_buff[j++] = dac_value * 256 / 2048;
//  }
//}

//**********************************************I2S_ADC**************************************************************************************

void i2s_adc(void *arg)
{
  unsigned long timeold = millis();

  for (int j = 0; j < waveDataSize / numPartWavData; ++j) {    //waveDataSize = record_time * 75680; numCommunicationData = 8000; numPartWavData = numCommunicationData / 4;

    I2S_Read(communicationData, numCommunicationData); //loop running 44 times

    for (int i = 0; i < numCommunicationData / 8; ++i) {
      partWavData[2 * i] = communicationData[8 * i + 2];
      partWavData[2 * i + 1] = communicationData[8 * i + 3];
      
      //      partWavData[2 * i] = (communicationData[8 * i + 1]>> val_shift) + val_offset + 8000;
      //      partWavData[2 * i + 1] = (communicationData[8 * i + 3]>> val_shift) + val_offset + 8000;
    }

    SerialBT.write((const byte*)partWavData, numPartWavData);
//    SerialBT.write('/n');





    //        SerialBT.write((const byte*)communicationData, numCommunicationData);
    Serial.printf("data written %d percent\n", j * 100 / (waveDataSize / numPartWavData));
    Serial.printf("Time Elapsed %d \n", (millis()-timeold)/1000);
//    ets_printf("Never Used Stack Size: %u\n", uxTaskGetStackHighWaterMark(NULL));
  }

  //  SerialBT.write((const byte*)c, sizeof(c));
  Serial.print("finish with total recording time being: ");
  Serial.println((millis() - timeold) / 1000);

  //    vTaskDelete(NULL);
  vTaskDelete(taskHandler);
}
//***************************************I2S_Buffer clean************************************************************************************
void i2s_cancel (void *arg) {
  Serial.println("*****Recording END 2******");
  vTaskDelay(2000);
  if (communicationData != NULL) {
    free(communicationData);
    communicationData = NULL;
  }

  if (partWavData != NULL) {
    free(partWavData);
    partWavData = NULL;
  }
  //  vTaskDelete(taskHandler);

  Serial.println("TAsk Del");
  i2s_driver_uninstall(I2S_PORT);
  Serial.println("Port uninstalled");
  vTaskDelete(NULL);
  Serial.println("Task del null");

}
