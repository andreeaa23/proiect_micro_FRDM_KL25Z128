#include "gpio.h"
#include "spi.h"
#include "uart.h"

int main(void) {
	  Init_SPI1();  
    OutputPIN_Init();
		UART0_Initialize(9600);

    Test_SPI_Loopback();  

    return 0;
}