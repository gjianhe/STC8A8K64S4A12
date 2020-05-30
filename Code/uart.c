#include <STC8.H>
#include "uart.h"

#define BUFFER_SIZE 128



typedef struct
{
    unsigned char ri, wi, ct, run;
    unsigned char  buff[BUFFER_SIZE];
} UARTFIFO;


#if UART1EN
static volatile UARTFIFO Uart1TxFifo;
static void Uart1Init(void)        //115200bps@22.1184MHz
{
    SCON = 0x50;        //8位数据,可变波特率
    AUXR |= 0x40;       //定时器1时钟为Fosc,即1T
    AUXR &= 0xFE;       //串口1选择定时器1为波特率发生器
    TMOD &= 0x0F;       //设定定时器1为16位自动重装方式
    TL1 = 0xD0;     //设定定时初值
    TH1 = 0xFF;     //设定定时初值
    ET1 = 0;        //禁止定时器1中断
    TR1 = 1;        //启动定时器1
    Uart1TxFifo.ri = 0;
    Uart1TxFifo.wi = 0;
    Uart1TxFifo.ct = 0;
    Uart1TxFifo.run = 0;
    ES = 1;
}

void Uart1PutC(unsigned char d)
{
    unsigned int i;

    while (Uart1TxFifo.ct >= BUFFER_SIZE) ;

    i = Uart1TxFifo.wi;
    Uart1TxFifo.buff[i++] = d;
    Uart1TxFifo.wi = i % BUFFER_SIZE;
    EA = 0;
    Uart1TxFifo.ct++;

    if (!Uart1TxFifo.run)
    {
        Uart1TxFifo.run = 1;
        TI = 1;
    }

    EA = 1;
}

void Uart1PutS(const char *st)
{
    while (*st)
    {
        Uart1PutC(*st++);
    }
}



void Uart1IRQ(void) interrupt 4
{
    if (TI)
    {
        unsigned int i;
        TI = 0;
        i = Uart1TxFifo.ct;

        if (i)
        {
            Uart1TxFifo.ct = --i;
            i = Uart1TxFifo.ri;
            SBUF = Uart1TxFifo.buff[i++];
            Uart1TxFifo.ri = i % BUFFER_SIZE;
        }
        else
        {
            Uart1TxFifo.run = 0;
        }
    }

    if (RI)
    {
        RI = 0;
    }
}

#endif


#if UART2EN
static volatile UARTFIFO Uart2TxFifo;
static void Uart2Init(void)     //115200bps@22.1184MHz
{
    S2CON = 0x50;       //8位数据,可变波特率
    AUXR |= 0x04;       //定时器2时钟为Fosc,即1T
    T2L = 0xD0;     //设定定时初值
    T2H = 0xFF;     //设定定时初值
    AUXR |= 0x10;       //启动定时器2
    Uart2TxFifo.ri = 0;
    Uart2TxFifo.wi = 0;
    Uart2TxFifo.ct = 0;
    Uart2TxFifo.run = 0;
    IE2 |= ES2;
}

void Uart2PutC(unsigned char d)
{
    unsigned int i;

    while (Uart2TxFifo.ct >= BUFFER_SIZE) ;

    i = Uart2TxFifo.wi;
    Uart2TxFifo.buff[i++] = d;
    Uart2TxFifo.wi = i % BUFFER_SIZE;
    EA = 0;
    Uart2TxFifo.ct++;

    if (!Uart2TxFifo.run)
    {
        Uart2TxFifo.run = 1;
        S2CON |= S2TI;
    }

    EA = 1;
}

void Uart2PutS(const char *st)
{
    while (*st)
    {
        Uart2PutC(*st++);
    }
}



void Uart2IRQ(void) interrupt 8
{
    if (S2CON & S2TI)
    {
        unsigned int i;
        S2CON &= ~S2TI;
        i = Uart2TxFifo.ct;

        if (i)
        {
            Uart2TxFifo.ct = --i;
            i = Uart2TxFifo.ri;
            S2BUF = Uart2TxFifo.buff[i++];
            Uart2TxFifo.ri = i % BUFFER_SIZE;
        }
        else
        {
            Uart2TxFifo.run = 0;
        }
    }

    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;
    }
}
#endif

#if UART3EN
static volatile UARTFIFO Uart3TxFifo;
void Uart3Init(void)        //115200bps@22.1184MHz
{
    S3CON = 0x10;       //8位数据,可变波特率
    S3CON |= 0x40;      //串口3选择定时器3为波特率发生器
    T4T3M |= 0x02;      //定时器3时钟为Fosc,即1T
    T3L = 0xD0;     //设定定时初值
    T3H = 0xFF;     //设定定时初值
    T4T3M |= 0x08;      //启动定时器3
    Uart3TxFifo.ri = 0;
    Uart3TxFifo.wi = 0;
    Uart3TxFifo.ct = 0;
    Uart3TxFifo.run = 0;
    IE2 |= ES3;
}

void Uart3PutC(unsigned char d)
{
    unsigned int i;

    while (Uart3TxFifo.ct >= BUFFER_SIZE) ;

    i = Uart3TxFifo.wi;
    Uart3TxFifo.buff[i++] = d;
    Uart3TxFifo.wi = i % BUFFER_SIZE;
    EA = 0;
    Uart3TxFifo.ct++;

    if (!Uart3TxFifo.run)
    {
        Uart3TxFifo.run = 1;
        S3CON |= S3TI;
    }

    EA = 1;
}

void Uart3PutS(const char *st)
{
    while (*st)
    {
        Uart3PutC(*st++);
    }
}



void Uart3IRQ(void) interrupt 17
{
    if (S3CON & S3TI)
    {
        unsigned int i;
        S3CON &= ~S3TI;
        i = Uart3TxFifo.ct;

        if (i)
        {
            Uart3TxFifo.ct = --i;
            i = Uart3TxFifo.ri;
            S3BUF = Uart3TxFifo.buff[i++];
            Uart3TxFifo.ri = i % BUFFER_SIZE;
        }
        else
        {
            Uart3TxFifo.run = 0;
        }
    }

    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI;
    }
}
#endif

#if UART4EN
static volatile UARTFIFO Uart4TxFifo;
void Uart4Init(void)        //115200bps@22.1184MHz
{
    S4CON = 0x10;       //8位数据,可变波特率
    S4CON |= 0x40;      //串口4选择定时器4为波特率发生器
    T4T3M |= 0x20;      //定时器4时钟为Fosc,即1T
    T4L = 0xD0;     //设定定时初值
    T4H = 0xFF;     //设定定时初值
    T4T3M |= 0x80;      //启动定时器4
    Uart4TxFifo.ri = 0;
    Uart4TxFifo.wi = 0;
    Uart4TxFifo.ct = 0;
    Uart4TxFifo.run = 0;
    IE2 |= ES4;
}

void Uart4PutC(unsigned char d)
{
    unsigned int i;

    while (Uart4TxFifo.ct >= BUFFER_SIZE) ;

    i = Uart4TxFifo.wi;
    Uart4TxFifo.buff[i++] = d;
    Uart4TxFifo.wi = i % BUFFER_SIZE;
    EA = 0;
    Uart4TxFifo.ct++;

    if (!Uart4TxFifo.run)
    {
        Uart4TxFifo.run = 1;
        S4CON |= S4TI;
    }

    EA = 1;
}

void Uart4PutS(const char *st)
{
    while (*st)
    {
        Uart4PutC(*st++);
    }
}



void Uart4IRQ(void) interrupt 18
{
    if (S4CON & S4TI)
    {
        unsigned int i;
        S4CON &= ~S4TI;
        i = Uart4TxFifo.ct;

        if (i)
        {
            Uart4TxFifo.ct = --i;
            i = Uart4TxFifo.ri;
            S4BUF = Uart4TxFifo.buff[i++];
            Uart4TxFifo.ri = i % BUFFER_SIZE;
        }
        else
        {
            Uart4TxFifo.run = 0;
        }
    }

    if (S4CON & S4RI)
    {
        S4CON &= ~S4RI;
    }
}
#endif

void UartInit(void)
{
    #if UART1EN
    Uart1Init();
    #endif
    #if UART2EN
    Uart2Init();
    #endif
    #if UART3EN
    Uart3Init();
    #endif
    #if UART4EN
    Uart4Init();
    #endif
}


