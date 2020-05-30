#ifndef __UART_H
#define __UART_H

#define UART1EN 1
#define UART2EN 0
#define UART3EN 0
#define UART4EN 0


void UartInit(void);

#if UART1EN
    void Uart1PutS(const char *st);
    void Uart1PutC(unsigned char d);
#endif

#if UART2EN
    void Uart2PutS(const char *st);
    void Uart2PutC(unsigned char d);
#endif

#if UART3EN
    void Uart3PutS(const char *st);
    void Uart3PutC(unsigned char d);
#endif

#if UART4EN
    void Uart4PutS(const char *st);
    void Uart4PutC(unsigned char d);
#endif




#endif
