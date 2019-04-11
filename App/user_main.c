/* 2019 04 10 */
/* By hxdyxd */

#include <stdlib.h>
#include "app_debug.h"
#include "soft_timer.h"
#include "user_main.h"
#include "usart.h"
#include "sdram_is42s16400j.h"
#include "fatfs.h"
#include "diskio.h"
#include "usb_host.h"
#include "gpio.h"


extern ApplicationTypeDef Appli_state;


void led_proc(void) 
{
    APP_DEBUG("usb status: %d \r\n", Appli_state);
    LED_REV(LED_BASE);
}


FATFS fatfs;
FIL fp;
uint8_t ff_buf[IS42S16400J_SIZE] __attribute__((at(SDRAM_BANK_ADDR)));

void udisk_proc(void)
{
    static ApplicationTypeDef last_status = APPLICATION_IDLE;
    int ret;
    UINT len = 0;
    
    if(last_status == Appli_state) {
        return;
    }
    last_status = Appli_state;
    switch(Appli_state) {
    case APPLICATION_START:
        APP_DEBUG("usb device connected \r\n");
        break;
    
    case APPLICATION_READY:
        APP_DEBUG("usb ready \r\n");
        //
        if((ret = f_mount(&fatfs, USBH_Path, 1)) != FR_OK) {
            APP_WARN("f_mount: %d \r\n", ret);
            break;
        }
        APP_DEBUG("%s mount success\r\n", USBH_Path);
        
        if((ret = f_open(&fp, "0:/hellof4_app.bin", FA_READ)) != FR_OK) {
            APP_WARN("f_open: %d \r\n", ret);
            break;
        }
        
        
        if((ret = f_read(&fp, ff_buf, 200000, &len)) != FR_OK) {
            APP_WARN("f_read: %d \r\n", ret);
            break;
        }
        APP_DEBUG("code length (%d)\r\n", len);
        
        f_close(&fp);
        
        if(1) {
            /************iap start****************/
            //Referance: STM32F4xx_AN3965
            uint32_t JumpAddress = *(__IO uint32_t*) (SDRAM_BANK_ADDR + 4);
            
            /* Jump to user application */
            typedef  void (*pFunction)(void);
            pFunction Jump_To_Application = (pFunction)JumpAddress;
            APP_DEBUG("jump to application 0x%p \r\n\r\n", Jump_To_Application);
            
            /* Disables interrupt  */
            HAL_NVIC_DisableIRQ(SysTick_IRQn);
            HAL_NVIC_DisableIRQ(DMA1_Stream0_IRQn);
            HAL_NVIC_DisableIRQ(DMA1_Stream6_IRQn);
            HAL_NVIC_DisableIRQ(DMA2_Stream7_IRQn);
            HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
                
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO uint32_t*)SDRAM_BANK_ADDR);
            Jump_To_Application();
            /************iap stop*****************/
        }
        break;
        
    case APPLICATION_DISCONNECT:
        APP_DEBUG("usb disconnect \r\n");
        if((ret = f_mount(NULL, (TCHAR *)USBH_Path, 1)) != FR_OK) {
            APP_WARN("f_mount: %d \r\n", ret);
        } else {
            APP_DEBUG("%s unmount success\r\n", USBH_Path);
        }
        break;
    default:
        ;
    }
}

void user_system_setup(void)
{
}

void user_setup(void)
{
    //turn off rgb led
    LED_OFF(LED_RED);
    LED_OFF(LED_GREEN);
    LED_OFF(LED_BLUE);
    
    //sdram init
    sdram_init();
    
    PRINTF("\r\n\r\n[HELLOF4 IAP] Build , %s %s \r\n", __DATE__, __TIME__);
    
    
    soft_timer_init();
    soft_timer_create(STIMER_LED_PROC_ID, 1, 1, led_proc, 500);
    
}

void user_loop(void)
{
    soft_timer_proc();
    udisk_proc();
}



/********************end of file*******************/
