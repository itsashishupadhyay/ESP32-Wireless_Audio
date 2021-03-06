#include "I2S.h"
#define I2S_PORT I2S_NUM_0
//#define I2S_PORT2 I2S_NUM_1
void I2S_Init(i2s_mode_t MODE, i2s_bits_per_sample_t BPS)
{
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = BPS,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
//    .communication_format = I2S_COMM_FORMAT_PCM,      // PCM
    .intr_alloc_flags = 0,
    .dma_buf_count = 16,
    .dma_buf_len = 64,
    //    .use_apll = 0,
    //    .tx_desc_auto_clear = false,
    //    .fixed_mclk = 0,
  };
  if (MODE == I2S_MODE_RX || MODE == I2S_MODE_TX) {
    Serial.println("using I2S_MODE");
    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = PIN_I2S_BCLK;
    pin_config.ws_io_num = PIN_I2S_LRC;
    if (MODE == I2S_MODE_RX) {
      pin_config.data_out_num = I2S_PIN_NO_CHANGE;
      pin_config.data_in_num = PIN_I2S_DIN;
    }
    else if (MODE == I2S_MODE_TX) {
      pin_config.data_out_num = PIN_I2S_DOUT;
      pin_config.data_in_num = I2S_PIN_NO_CHANGE;
    }

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
    REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);
    i2s_set_pin(I2S_PORT, &pin_config);
    i2s_set_clk(I2S_PORT, SAMPLE_RATE, BPS, I2S_CHANNEL_STEREO);



  }
  else if (MODE == I2S_MODE_DAC_BUILT_IN || MODE == I2S_MODE_ADC_BUILT_IN) {
    Serial.println("using ADC_builtin");
    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    //     i2s_driver_install(I2S_PORT2, &i2s_config, 0, NULL);
    // GPIO36, VP
    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
    //    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_3);
  }
}

int I2S_Read(char *data, int numData)
{
  size_t bytes_read;
  //  Serial.println("I2S read ");
  //  return i2s_read_bytes(I2S_PORT, (char *)data, numData, portMAX_DELAY);
  return i2s_read(I2S_PORT, (char *)data, numData, &bytes_read, portMAX_DELAY);
  //  Serial.println("I2S read returned");

}

void I2S_Write(char *data, int numData)
{
  i2s_write_bytes(I2S_PORT, (const char *)data, numData, portMAX_DELAY);
}
