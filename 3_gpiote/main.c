#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"

void button_init(){ 
	GPIO->PIN_CNF[13] = (0 << 0) | (3 << 2);
}

// Configure LED Matrix
void led_init(){
	for(int i = 17; i <= 20; i++){
		GPIO->DIRSET = (1 << i);
        GPIO->PIN_CNF[i] = (1 << 0);
		GPIO->OUTCLR = (1 << i);
	}
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

void gpiote_init() {
    // Configure channel 0 to listen for button 1 (pin 13)
    // MODE, PSEL, POLARITY
    GPIOTE->CONFIG[0] = (1 << 0) | (13 << 8) | (2 << 16);

    // Clear event
    GPIOTE->EVENTS_IN[0] = 0;

    // Configure channel 1 for using LED 1 
    // MODE, PSEL, POLARITY
    GPIOTE->CONFIG[1] = (3 << 0) | (17 << 8) | (3 << 16);

    // Configure channel 2 for using LED 2
    // MODE, PSEL, POLARITY
    GPIOTE->CONFIG[2] = (3 << 0) | (18 << 8) | (3 << 16);

    // Configure channel 3 for using LED 3 
    // MODE, PSEL, POLARITY
    GPIOTE->CONFIG[3] = (3 << 0) | (19 << 8) | (3 << 16);

    // Configure channel 4 for using LED 4
    // MODE, PSEL, POLARITY
    GPIOTE->CONFIG[4] = (3 << 0) | (20 << 8) | (3 << 16);
}

void ppi_init() {
    //PPI->PPI_TASKS[0].EN = 1;
    
    PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[0].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[1]);

    PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[2]);

    PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[2].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[3]);

    PPI->PPI_CH[3].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
    PPI->PPI_CH[3].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[4]);

    //PPI->CHEN = (1 << 0);
    PPI->CHENSET = (1 << 0);

    //PPI->CHEN = (1 << 1);
    PPI->CHENSET = (1 << 1);

    //PPI->CHEN = (1 << 2);
    PPI->CHENSET = (1 << 2);

    //PPI->CHEN = (1 << 3);
    PPI->CHENSET = (1 << 3);

}

int main() {

    button_init();

    led_init();

    gpiote_init();   

    ppi_init();

    while(1);

    return 0;
}