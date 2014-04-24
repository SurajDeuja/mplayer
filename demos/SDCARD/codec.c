#include "codec.h"

const stm32_dma_stream_t *i2sdma;
static uint32_t i2stxdmamode = 0;

static const I2CConfig i2cfg = {
	OPMODE_I2C,
	100000,
	FAST_DUTY_CYCLE_16_9 /* find this*/
};

#define I2S3_TX_DMA_CHANNEL	\
	STM32_DMA_GETCHANNEL(STM32_SPI_SPI3_TX_DMA_STREAN, \
			     STM32_SPI_TX_DMA_CHN)

