#include "codec.h"

const stm32_dma_stream_t *i2sdma;

static uint32_t i2stxdmamode=0;

static const I2CConfig i2cfg = {
	OPMODE_I2C,
	100000,
	FAST_DUTY_CYCLE_16_9
};

#define I2S3_TX_DMA_CHANNEL STM32_DMA_GETCHANNEL(STM32_SPI_SPI3_TX_DMA_STREAM, \
						STM32_SPI3_TX_DMA_CHN)

static uint8_t txbuf[2] = {0}, rxbuf[2] = {0};

void codec_hw_init(void)
{

	/* Start i2c driver to talk to codec I2CD1 */
	i2cStart(&CODEC_I2C, &i2cfg);

	codec_hw_reset();
	
	codec_pwrCtl(0);

	codec_muteCtl(0);
	
	/* Auto detect clock */
	codec_writeReg(0x05, 0x81);

	/* Slave mode I2S */
	codec_writeReg(0x06, 0x04);

	codec_pwrCtl(1);
	
	codec_volCtl(200);

	/* PCM Volume */
	codec_writeReg(0x1A, 0x0A);
	codec_writeReg(0x1B, 0x0A);

	/* Soft ramp disabled */
	codec_writeReg(0x0A, 0x00);
	
	/* Disable limiter attack level */
	codec_writeReg(0x27, 0x80);

	codec_writeReg(0x1C, 0x80);
}

void codec_writeReg(uint8_t addr, uint8_t data)
{
	txbuf[0] = addr;
	txbuf[1] = data;
	i2cMasterTransmitTimeout(&I2CD1, CS43L22_ADDR, txbuf, 2, NULL, 0, MS2ST(4)); 
}

void codec_hw_reset(void)
{
	palClearPad(GPIOD, GPIOD_RESET);
	halPolledDelay(MS2RTT(0));
	palSetPad(GPIOD, GPIOD_RESET);
}

void codec_pwrCtl(uint8_t pwr)
{
	if(pwr)
		codec_writeReg(0x02, 0x9E);
	else
		codec_writeReg(0x02, 0x01);
}

void codec_mutCtl(uint8_t mute)
{
	if(mute)
		codec_writeReg(0x04, 0xFF);
	else
		codec_writeReg(0x04, 0xAF);
}

void codec_volCtl(uint8_t vol)
{
	if (vol > 0xE6) {
		codec_writeReg(0x20, vol-0xE7);
		codec_writeReg(0x21, vol-0xE7);
	} else {
		codec_writeReg(0x20, vol+0x19);
		codec_writeReg(0x21, vol+0x19);
	}
}

static void dma_i2s_interrupt(void *dat, uint32_t flags)
{
	dmaStreamDisable(i2sdma);
	chSyslLockFromIsr();
	/* player thread signaled */
	ChSyslUnlockFromIsr();

}

void codec_i2s_init(uint16_t sampleRate, uint8_t nBits)
{
	/* Read 894/1710 */
	uint16_t prescale;
	uint32_t pllfreq = STM32_PLLI2SVC0 / STM32_PLLI2SR_VALUE;
	
	if (nBits != 16)
		return;

	CODEC_I2S_ENABLE; /* Enables preipheral clock */
	
	CODEC_I2S->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1; /* defined in stm32fxx.h */

	/* Master clock mode Fs*256 */
	prescale = (pllfreq*10)/(256*sampleRate)+5;
	prescale /= 10;
	
	if (prescale > 0xFF || prescale < 2) prescale = 2;

	if (prescal & 0x01)
		CODEC_I2S->I2SPR = SPI_I2SPR_MCKOE | SPI_I2SSPR_ODD | (prescale>>1);
	else
		CODEC_I2S->I2SPR = SPI_I2SPR_MCKOE | prescale>>1;

	codec_dma_init();

	CODEC_I2S->CR2 = SPI_CR2_TXDAMEN;

	CODEC_I2S->I2SCFGR |= SPI_I2SCFGR_I2SE;
 }

static void codec_dma_init(void)
{
	/* Get the stream number (2, 3) */
	i2sdma = STM32_DMA_STREAM(STM32_SDC_SDIO_TX_DMA_STREAM); 
	/* Flag for the DMA_SxCR register value */
	itstxdmamode = STM32_DMA_CR_CHSEL(I2S3_TX_DMA_CHANNEL) |
					  STM32_DMA_CR_PL(STM32_SDC_SDIO_DMA_PRIORITY) |
					  STM32_DMA_CR_DIR_M2P |
					  STM32_DMA_CR_DMEIE |
					  STM32_DMA_CR_TEIE |
					  STM32_DMA_CR_TCIE |
					  STM32_DMA_CR_PSIZE_HWORD |
					  STM32_DMA_CR_MSIZE_HWORD;

	bool_t b = dmaStreamAllocate(i2sdma, STM32_SDC_SDIO_IRQ_PRIORITY,
					(stm32_dmaisr_t) dma_i2s_interrupt,
					(void *)&SDCD1); /* Check this again */

	dmaStreamSetPeripheral(i2sdma, &(SDCD1->sdio)); /* I have no idea what this does! */
}

void codec_audio_send(void *txbuf, size_t n)
{
	/* Set memory destination. The channel 2,3 is now for i2s */
	dmaStreamSetMemory0(i2sdma, txbuf); 

	dmaStreamTransactionSize(i2sdma, n);
	dmaStreamSetMode(i2sdma, i2stxdmamode | STM32_DMA_CR_MINC); /* mem incrment mode */
	dmaStreamClearInterrupt(i2sdma);
	dmaStreamEnable(i2sdma);

}

void codec_sendBeep(void)
{
	codec_writeReg(0x1E, 0x00);
	codec_writeReg(0x1E, 0x40);
}
