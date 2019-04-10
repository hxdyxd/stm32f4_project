#ifndef _USER_MAIN_H_
#define _USER_MAIN_H_

#define STIMER_LED_PROC_ID 0


/* LEDS */
#define LED_OFF(id)  HAL_GPIO_WritePin(id, GPIO_PIN_SET)
#define LED_ON (id)  HAL_GPIO_WritePin(id, GPIO_PIN_SET)
#define LED_REV(id)  HAL_GPIO_TogglePin(id)

#define LED_BASE   GPIOD, GPIO_PIN_12
#define LED_RED    GPIOH, GPIO_PIN_10
#define LED_GREEN  GPIOH, GPIO_PIN_11
#define LED_BLUE   GPIOH, GPIO_PIN_12

/* tea5767 */
#define TEA5767_DEVICE_ID_8BIT   (0x60<<1)


void user_setup(void);
void user_loop(void);


#endif
