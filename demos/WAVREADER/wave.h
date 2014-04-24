#define WAVE_HEAD_RIFF		0x46464952	/* RIFF in little endian */
#define	WAVE_HEAD_WAVE		0x45564157	/* WAVE in little endian */
#define WAVE_HEAD_FMT		0x20746d66	/* fmt + ' ' */
#define WAVE_HEAD_DATA		0x61746164	/* data in little endian */

#define WAVE_META_LIST		0x5453494c
#define WAVE_META_INFO		0x4f464e49
#define WAVE_META_INAM		0x4d414e49
#define WAVE_META_IART		0x54524149
#define WAVE_META_ICMT		0x544d4349
#define WAVE_META_ICRD		0x44524349

typedef struct wavefile {
	uint32_t riffSignature;
	uint32_t fileSize;
	uint32_t waveSignature;
	uint32_t fmtSignature;
	uint32_t subChunk1Size;
	uint16_t format;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t numBits;
	uint32_t meta_data;
	uint32_t wavLength;
} WAVEFILE;
