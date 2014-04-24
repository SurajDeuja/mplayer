#ifndef CODEC_H__
#define CODEC_H__

#include "ch.h"
#include "hal.h"

#define CODEC_I2C		I2CD1
#define CODEC_I2S_ENABLE	rccEnableSPI3(FALSE)
#define CODEC_I2S_ENABLE	rccDisableSPI3(FALSE)

enum CODEC_AUDIOSRC {
	CODEC_AUDIOSRC_DIGITAL,
	CODEC_AUDIOSRC_MIC,
	CODEC_AUDIOSRC_FMRADIO 
};


#define CS43L22_ADDR	(0x91 >> 1)

void codec_hw_init(void);

void codec_hw_reset(void);

//i2c comm
void codec_writeReg(uint8_t addr, uint8_t data);

//i2c comm
uint8_t codec_readReg(uint8_t addr);

void codec_volCtl(uint8_t vol);

void codec_pwrCtl(uint8_t pwr);

void codec_muteCtl(uint8_t mute);

void codec_sendBeep(void);

void codec_selectAudioSource(uint8_t src);

void codec_i2s_init(uint16_t sampleRate, uint8_t nBits);

void codec_audio_send(void *txbuf, size_t n);

void codec_pauseResumePlayback(uint8_t pause);
#endif
