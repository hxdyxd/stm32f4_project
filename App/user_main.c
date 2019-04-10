/* 2019 04 10 */
/* By hxdyxd */

#include "app_debug.h"
#include "soft_timer.h"
#include "user_main.h"
#include "usart.h"
#include "i2c.h"
#include "sdram_is42s16400j.h"
#include "fatfs.h"
#include "diskio.h"
#include "usb_host.h"


unsigned char *radio_freq_set(unsigned char radio[5], double frequency)
{
    unsigned char frequencyH = 0;
    unsigned char frequencyL = 0;

    unsigned int frequencyB;
    //double frequency = strtod(argv[1], NULL);

    frequencyB = 4*(frequency*1000000+225000)/32768; //calculating PLL word
    frequencyH = frequencyB >> 8;
    frequencyL = frequencyB & 0XFF;

    // data to be sent
    radio[0]=frequencyH; //FREQUENCY H
    radio[1]=frequencyL; //FREQUENCY L
    radio[2]=0xB0; //3 byte (0xB0): high side LO injection is on,.
    radio[3]=0x10; //4 byte (0x10) : Xtal is 32.768 kHz
    radio[4]=0x00; //5 byte0x00)
    return radio;
}

static unsigned char radio[5] = {0};
extern ApplicationTypeDef Appli_state;

void led_proc(void) 
{
    *((uint32_t *)SDRAM_BANK_ADDR) = 0xaabbccdd;
    APP_DEBUG("hello world %x\r\n", *((uint32_t *)(SDRAM_BANK_ADDR)) );
    APP_DEBUG("usb status: %d \r\n", Appli_state);
    LED_REV(LED_BASE);
}

FATFS fatfs;
FIL fp;
uint8_t ff_buf[200] __attribute__((at(SDRAM_BANK_ADDR)));

void udisk_proc(void)
{
    static ApplicationTypeDef last_status = APPLICATION_IDLE;
    int ret;
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
        
        if((ret = f_open(&fp, "0:/test.txt", FA_READ)) != FR_OK) {
            APP_WARN("f_open: %d \r\n", ret);
            break;
        }
        
        UINT len = 0;
        if((ret = f_read(&fp, ff_buf, 200, &len)) != FR_OK) {
            APP_WARN("f_read: %d \r\n", ret);
            break;
        }
        ff_buf[len] = '\0';
        APP_DEBUG("%.*s (%d)\r\n", len, ff_buf, len);
        
        f_close(&fp);
        
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


void user_setup(void)
{
    //sdram init
    sdram_init();
    
    //turn off rgb led
    LED_OFF(LED_RED);
    LED_OFF(LED_GREEN);
    LED_OFF(LED_BLUE);
    
    //tea5767 init
    HAL_I2C_Master_Transmit(&hi2c1, TEA5767_DEVICE_ID_8BIT, radio_freq_set(radio, 105.1), 5, 100);
    
    
    soft_timer_init();
    
    soft_timer_create(STIMER_LED_PROC_ID, 1, 1, led_proc, 500);
    
    
    
    int ret;
    
    /*if((ret = f_mount(&fatfs, (TCHAR *)SD_Path, 1)) != FR_OK) {
        APP_WARN("f_mount: %d \r\n", ret);
    }*/
    
}

void user_loop(void)
{
    soft_timer_proc();
    udisk_proc();
}



/********************end of file*******************/
