#include <stdio.h>
#include "usart.h"

int fputc(int ch, FILE *f)
{
    uint8_t c = ch;
    HAL_UART_Transmit(&huart1, &c, 1, 200);
    return ch;
}

int fgetc(FILE *f)
{
    return 1;
}
