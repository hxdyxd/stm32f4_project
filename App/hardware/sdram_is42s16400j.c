/* 2019 04 10 */
/* By hxdyxd */

#include <string.h>
#include "fmc.h"
#include "sdram_is42s16400j.h"


void sdram_init(void)
{
    FMC_SDRAM_CommandTypeDef fmc_cmd;
    
    fmc_cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    fmc_cmd.AutoRefreshNumber = 0;
    fmc_cmd.ModeRegisterDefinition = 0;
    HAL_SDRAM_SendCommand(&hsdram1, &fmc_cmd, 100);
    
    uint32_t delay = 100000;
    while(--delay);
    
    fmc_cmd.CommandMode = FMC_SDRAM_CMD_PALL;
    fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    fmc_cmd.AutoRefreshNumber = 0;
    fmc_cmd.ModeRegisterDefinition = 0;
    HAL_SDRAM_SendCommand(&hsdram1, &fmc_cmd, 100);
    
    fmc_cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    fmc_cmd.AutoRefreshNumber = 2;
    fmc_cmd.ModeRegisterDefinition = 0;
    HAL_SDRAM_SendCommand(&hsdram1, &fmc_cmd, 100);
    
    fmc_cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    fmc_cmd.AutoRefreshNumber = 1;
    fmc_cmd.ModeRegisterDefinition = 
                   SDRAM_MODEREG_BURST_LENGTH_4 |
                   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                   SDRAM_MODEREG_CAS_LATENCY_2           |
                   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
    HAL_SDRAM_SendCommand(&hsdram1, &fmc_cmd, 100);
    
    while(HAL_SDRAM_ProgramRefreshRate(&hsdram1, 1386) != HAL_OK)
    {
        printf("sdram error set refresh rate \r\n");
    }
    
    sdram_test_8();
    sdram_test_16();
    sdram_test_32();
    sdram_test_64(IS42S16400J_SIZE - 32*sizeof(double) );
    //fill zero to memary
    memset((void *)SDRAM_BANK_ADDR, 0, IS42S16400J_SIZE);
}

void sdram_test_8(void)
{
    int i;
    uint8_t *buf8 = (uint8_t *)SDRAM_BANK_ADDR;
    
    printf("\r\n**********[sdram test]**********\r\n");
    for(i=0; i<32; i++) {
        buf8[i] = i;
    }
    for(i=0; i<32; i++) {
        printf("%03d ", buf8[i]);
        if(i && (i+1)%0x10 == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n**********[sdram test]**********\r\n");
}

void sdram_test_16(void)
{
    int i;
    uint16_t *buf16 = (uint16_t *)SDRAM_BANK_ADDR;
    
    printf("\r\n**********[sdram test]**********\r\n");
    for(i=0; i<32; i++) {
        buf16[i] = i|(i<<8);
    }
    for(i=0; i<32; i++) {
        printf("%04x ", buf16[i]);
        if(i && (i+1)%0x8 == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n**********[sdram test]**********\r\n");
}

void sdram_test_32(void)
{
    int i;
    uint32_t *buf32 = (uint32_t *)SDRAM_BANK_ADDR;
    
    printf("\r\n**********[sdram test]**********\r\n");
    for(i=0; i<32; i++) {
        buf32[i] = i|(i<<8)|(i<<16)|(i<<24);
    }
    for(i=0; i<32; i++) {
        printf("%08x ", buf32[i]);
        if(i && (i+1)%0x8 == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n**********[sdram test]**********\r\n");
}

void sdram_test_64(uint32_t addr)
{
    int i;
    double *buf64 = (double *)(SDRAM_BANK_ADDR + addr);
    
    printf("\r\n**********[sdram test]**********\r\n");
    for(i=0; i<32; i++) {
        buf64[i] = i*1.0;
    }
    for(i=0; i<32; i++) {
        printf("%.2f ", buf64[i]);
        if(i && (i+1)%0x8 == 0) {
            printf("\r\n");
        }
    }
    printf("\r\n**********[sdram test]**********\r\n");
}








/*****************************END OF FILE****************************/
