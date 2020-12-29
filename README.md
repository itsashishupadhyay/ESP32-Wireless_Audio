# ESP32-Wireless_Audio

I was trying to create a system where I can reliably send live audio from ESP to my laptop with Audio Quality intact.
The ESP code is culmination of lots of codes and guidance through myriads of forums with me only tweaking the provided libraries.

##Prerequsite
__Software__
- Arduino with ESP32 Setup
- Audacity
- Python

__Hardware__
- Adafruit ESP32 Huzzah is Used Here
- MAX4466 with amplifier (I used a Module)
- SD card module if you want to save it locally

__NOTE:__ I was able to test it with Analog Mic over Bluetooth with my python code. I couldn’t get it to work with SPH0645LM4H so if someone did please reach out to me, I would love to here it out!
However the code can be use with **ADMP441** if stereo/Digital configuration is desired.

##Firmware

I decided to use the Wave Codec due to my familiarity, however you can send the raw data and cover it using Audacity, if you don’t want to receive the wave audio file (using the provided Python Code).

This has two version on coding part either you can send raw data or the data with wave header, latter can be directly played while raw will need Audacity. The audio config are as follows.
- 16-bit
- Little endian
- 44100

The library is written keeping I2S protocol in mind so 
	#define SAMPLE_RATE (44100)
	#define PIN_I2S_BCLK 26
	#define PIN_I2S_LRC 22
	#define PIN_I2S_DIN 34
	#define PIN_I2S_DOUT 25
`PIN_I2S_DOUT` for analog is connected to analog Out of the MIC 
__Uncomment__
` #define I2S_MODE I2S_MODE_ADC_BUILT_IN `

__This will enable the code segment__

	else if (MODE == I2S_MODE_DAC_BUILT_IN || MODE == I2S_MODE_ADC_BUILT_IN) {
	Serial.println("using ADC_builtin");
	i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
	// GPIO36, VP
	i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
	}

Also the Audio Config ,

	const int record_time = 1*60;  // second
	const int headerSize = 44;     //Wave Header
	const int waveDataSize = record_time * 75680; //88000; // (44100 hz, 16 	bit and 1 channel)/8 75680

##Audacity

In Audacity 
Go to File > Import > Raw data and 
- signed 16-bit PCM
- Little endian
- 44100
- Serio or Mono dependent on your ESP configuration


