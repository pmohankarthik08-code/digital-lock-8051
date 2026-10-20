/* Digital Lock - 8051 Embedded C
   Compiler target: SDCC MCS-51
   Password: 1234
   Input: UART serial, 4 digits
*/

__sfr __at (0xA0) P2;
__sfr __at (0x98) SCON;
__sfr __at (0x89) TMOD;
__sfr __at (0x8D) TH1;
__sfr __at (0x8B) TL1;
__sfr __at (0x99) SBUF;

__sbit __at (0x8E) TR1;
__sbit __at (0x98) RI;
__sbit __at (0x99) TI;
__sbit __at (0xA0) LOCK_LED;
__sbit __at (0xA1) ERROR_LED;

#define MAX_ATTEMPTS 3

unsigned char attempts = 0;
unsigned char input[5];

void delay(void)
{
    unsigned int x, y;

    for (x = 0; x < 500; x++)
        for (y = 0; y < 120; y++);
}

void uart_init(void)
{
    SCON = 0x50;
    TMOD = (TMOD & 0x0F) | 0x20;
    TH1 = 0xFD;
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

void main(void)
{
    unsigned char count;
    char c;

    LOCK_LED = 0;
    ERROR_LED = 0;
    uart_init();

    uart_send_string("\r\n8051 DIGITAL LOCK\r\n");
    uart_send_string("Enter password:\r\n");

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

        if (input[0] == '1' &&
            input[1] == '2' &&
            input[2] == '3' &&
            input[3] == '4')
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
                uart_send_string("LOCKOUT\r\n");

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
