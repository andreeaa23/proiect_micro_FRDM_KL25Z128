#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "adc.h"

int main(void) {
	  //Init_SPI1();  
    //OutputPIN_Init();
		UART0_Init(115200);
		ADC0_Init();
	//OutputPIN_Init();
		for(;;){}
    //Test_Loopback();  

    return 0;
}