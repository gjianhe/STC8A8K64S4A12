#include <STC8.H>
#include "uart.h"

void main(void)
{
    UartInit();
    EA = 1;

    while (1)
    {
        Uart1PutS("2020-05-30-12:08，周六，小雨，广东深圳龙华\r\n");
    }
}



