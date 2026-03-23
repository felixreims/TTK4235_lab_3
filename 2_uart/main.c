#include "gpio.h"
#include "uart.h"

#include <stdio.h>
#include <sys/types.h> // For ssize_t


#define GPIO ((NRF_GPIO_REG*)0x50000000)

#define BIT(x) (1U << (x))

void button_init(){ 
	GPIO->PIN_CNF[13] = (0 << 0) | (3 << 2);
    
	GPIO->PIN_CNF[14] = (0 << 0) | (3 << 2);
	// Fill inn the configuration for the remaining buttons 
}

void set_leds() {
    for(int i = 17; i <= 20; i++){
        GPIO->OUTSET = (1 << i);
    }
}

void clear_leds() {
    for(int i = 17; i <= 20; i++){
        GPIO->OUTCLR = (1 << i);
    }
}

ssize_t _write(int fd, const void *buf, size_t count){
    char * letter = (char *)(buf);
    for(int i = 0; i < count; i++){
        uart_send(*letter);
        letter++;
    }
    return count;
}

int main(){
	// Configure LED Matrix
	for(int i = 17; i <= 20; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	// Configure buttons -> see button_init()
	button_init();

    uart_init();

	int sleep = 0;

    uint8_t led_on = 0;

    iprintf("The average grade in TTK%d was in %d was: %c\n\r",4235,2022,'B');

	while(1){

		// Check if button 1 is pressed
		if ( !(GPIO->IN & BIT(13)) ) {
            if (led_on) {
                led_on = 0;
                clear_leds();
            }
            uart_send('A');
		}

		// Check if button 2 is pressed
		if ( !(GPIO->IN & BIT(14)) ) {
            if (!led_on) {
                led_on = 1;
                set_leds();
            }
            uart_send('B');
		}

        if (uart_read() != '\0') {
            if (led_on) {
                led_on = 0;
                clear_leds();
            }

            else if (!led_on) {
                led_on = 1;
                set_leds();
            }
        }

		sleep = 10000;
		while(--sleep); // Delay
	}
	return 0;
}
