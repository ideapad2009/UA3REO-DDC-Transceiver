#ifndef AGC_H
#define AGC_H

#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include "audio_processor.h"

//Public methods
extern void DoRxAGC(float32_t *agcbuffer, uint_fast16_t blockSize, AUDIO_PROC_RX_NUM rx_id); // start AGC on a data block
extern void ResetAGC(void);

#endif
