#include "gpio.h"
#include "uart.h"

void uart_init() {

	GPIO->PIN_CNF[6] = (1 << 0); // TXD, output
	GPIO->PIN_CNF[8] = (0 << 0); // RXD, input

    UART->PSELTXD = 6;
    UART->PSELRXD = 8;

    UART->BAUDRATE = 0x00275000; // Baud9600

    UART->PSELRTS = 0xFFFFFFFF; // Disconnected
    UART->PSELCTS = 0xFFFFFFFF; // Disconnected

    UART->ENABLE = 4; // Enabled

    UART->TASKS_STARTRX = 1;
}


void uart_send(char letter) {

    UART->TXD = (uint32_t)letter;

    UART->TASKS_STARTTX = 1;
    
    while(!UART->EVENTS_TXDRDY); // Poll for TX to be done

    UART->EVENTS_TXDRDY = 0; // Clear the interrupt
    UART->TASKS_STOPTX = 1;
}


char uart_read() {

    if (UART->EVENTS_RXDRDY) {
        UART->EVENTS_RXDRDY = 0;
        char c = (char)UART->RXD;
        UART->TASKS_STARTRX = 1;
        return c;
    }

    return '\0';
   
}

