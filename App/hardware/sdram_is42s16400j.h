/* 2019 04 10 */
/* By hxdyxd */
#ifndef _SDRAM_IS42_H_
#define _SDRAM_IS42_H_

#include <stdint.h>


#define IS42S16400J_SIZE 0x800000  //400000*16bits = 0x800000  ，8M bytes

//enable xip on sdram
#define SWP_FMC_ON

#ifdef SWP_FMC_ON
#define SDRAM_BANK_ADDR     ((uint32_t)0x90000000)
#else
#define SDRAM_BANK_ADDR     ((uint32_t)0xD0000000)
#endif

/**
  * @brief  FMC SDRAM 模式配置的寄存器相关定义
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)


void sdram_init(void);

void sdram_test_8(void);
void sdram_test_16(void);
void sdram_test_32(void);
void sdram_test_64(uint32_t addr);

#endif
/*****************************END OF FILE****************************/
