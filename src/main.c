#include "gpio.h"
#include "uart.h"
#include "adc.h"

int main(void) {
 
    //OutputPIN_Init();
		UART0_Init(115200);
		ADC0_Init();
	//OutputPIN_Init();
		for(;;){} 

    return 0;
}