#include "spi.h"
#include "gpio.h"
#include "uart.h"

void Init_SPI1(void){
	//enable clock to SPI1
	SIM->SCGC4 |= SIM_SCGC4_SPI1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//disable SPI1 to allow configuration
	SPI1->C1 &= ~SPI_C1_SPE_MASK;
	
	//set PTE2 as SPI1_SCK --ALT2
	PORTE->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[2] |= PORT_PCR_MUX(2);
	//set PTE3 as SPI1_MOSI --ALT5
	PORTE->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[3] |= PORT_PCR_MUX(5);
	//set PTE1 as SPI1_MISO --ALT5
	PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[1] |= PORT_PCR_MUX(5);
	//set PTE4 as SPI1_PCS0 --ALT2
	PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[4] |= PORT_PCR_MUX(2);
	
	//Select master mode, enable SS output
	SPI1->C1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;
	SPI1->C2 = SPI_C2_MODFEN_MASK;
	//Select active high clock, first edge sample
	SPI1->C1 &= ~SPI_C1_CPHA_MASK;
	SPI1->C1 &= ~SPI_C1_CPOL_MASK;
	
	//BaudRate = BusClock / ((SPPR+1)*2^(SPR+1))
	SPI1->BR = SPI_BR_SPPR(2) | SPI_BR_SPR(1);
	
	//enable SPI1
	SPI1->C1 |= SPI_C1_SPE_MASK;
}

uint8_t Test_SPIsend(uint8_t d_out)
{
	while(!(SPI1->S & SPI_S_SPTEF_MASK))
		; //Wait for transmit buffer empty
	SPI1->D=d_out;
	
	while(!(SPI1->S & SPI_S_SPRF_MASK))
		; //wait for receive buffer full
	return SPI1->D;
}

void Test_SPI_Loopback(void)
{
	uint8_t in;
	while (1)
	{
    in = UART0_Receive();
    if (in == 'R' | in =='r')
    {
        // Red: error, data doesn't match
        Control_RGB_LEDs(1, 0, 0, 0); // Turn off the black LED
    }
    else if (in == 'B')
    {
        Control_RGB_LEDs(0, 0, 1, 0); // Turn on the black LED
    }
		else if (in == 'T'){
			Control_RGB_LEDs(0, 0, 0, 1);
		}
    else
    {
        // Green: data matches
        Control_RGB_LEDs(0, 1, 0, 0); // Turn off the black LED
    }
    UART0_Transmit(in);
	}
}
