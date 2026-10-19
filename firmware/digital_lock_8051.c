#include <reg51.h>

/* Digital Lock - 8051 Microcontroller
   Password: 1234
   Keypad input is simulated using UART serial input.
*/

sbit LOCK_LED = P2^0;
sbit ERROR_LED = P2^1;

#define PASSWORD "1234"
#define MAX_ATTEMPTS 3

unsigned char attempts = 0;
unsigned char i;
char input[5];

void delay(void)
{
    unsigned int x, y;
    for (x = 0; x < 500; x++)
        for (y = 0; y < 120; y++);
}

void uart_init(void)
{
    SCON = 0x50;
    TMOD = 0x20;
    TH1 = 0xFD;       /* 9600 baud at 11.0592 MHz */
    TL1 = 0xFD;
    TR1 = 1;
}

char uart_receive(void)
{
    char c;
    while (RI == 0);
    c = SBUF;
    RI = 0;
    return c;
}

void uart_send_char(char c)
{
    SBUF = c;
    while (TI == 0);
    TI = 0;
}

void uart_send_string(char *s)
{
    while (*s)
        uart_send_char(*s++);
}

bit check_password(char *entered)
{
    i = 0;
    while (PASSWORD[i] != '\0' && entered[i] == PASSWORD[i])
        i++;

    return (PASSWORD[i] == '\0' && entered[i] == '\0');
}

void main(void)
{
    unsigned char count;
    char c;

    LOCK_LED = 0;
    ERROR_LED = 0;
    uart_init();

    uart_send_string("\r\n8051 DIGITAL LOCK\r\n");
    uart_send_string("Enter password 1234:\r\n");

    while (1)
    {
        count = 0;

        while (count < 4)
        {
            c = uart_receive();

            if (c >= '0' && c <= '9')
            {
                input[count] = c;
                uart_send_char('*');
                count++;
            }
        }

        input[4] = '\0';
        uart_send_string("\r\n");

        if (check_password(input))
        {
            attempts = 0;
            LOCK_LED = 1;
            ERROR_LED = 0;

            uart_send_string("ACCESS GRANTED\r\n");
            delay();

            LOCK_LED = 0;
            uart_send_string("LOCKED AGAIN\r\n");
        }
        else
        {
            attempts++;
            ERROR_LED = 1;
            uart_send_string("WRONG PASSWORD\r\n");

            if (attempts >= MAX_ATTEMPTS)
            {
                uart_send_string("3 FAILURES - LOCKOUT\r\n");
                while (1)
                {
                    LOCK_LED = 0;
                    ERROR_LED = 1;
                }
            }

            ERROR_LED = 0;
            uart_send_string("TRY AGAIN\r\n");
        }
    }
}
