#include "gpio.h"
#include "uart.h"

#define RED_LED_PIN (18) // PORT B
#define GREEN_LED_PIN (19) // PORT B
#define BLUE_LED_PIN (1) // PORT D
#define BLACK_LED_PIN (2) //PORT D

void OutputPIN_Init(void){
	
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	
	// --- RED LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<RED_LED_PIN); 
	
	// --- GREEN LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<GREEN_LED_PIN);  //scriu 1 stinge ledul
	
	// --- BLUE LED ---
	
		// Utilizare GPIO ca varianta de multiplexare
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOD_PDDR |= (1<<BLUE_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOD_PSOR |= (1<<BLUE_LED_PIN);  //set e stins
	// ---- BLACK LED ----
	// Utilizare GPIO ca varianta de multiplexare
PORTD->PCR[BLACK_LED_PIN] &= ~PORT_PCR_MUX_MASK;
PORTD->PCR[BLACK_LED_PIN] |= PORT_PCR_MUX(1);

// Configurare pin pe post de output
GPIOD_PDDR |= (1 << BLACK_LED_PIN);

// Stingerea LED-ului (punerea pe 0 logic)
GPIOD_PSOR |= (1 << BLACK_LED_PIN);
}

void Control_RGB_LEDs(uint8_t red, uint8_t green, uint8_t blue, uint8_t black) {
    // Setare sau stinge LED-ul rosu
    if (red) {
        GPIOB_PCOR |= (1 << RED_LED_PIN);  // Seteaza pinul
    } else {
        GPIOB_PSOR |= (1 << RED_LED_PIN);  // Stinge pinul
    }

    // Setare sau stinge LED-ul verde
    if (green) {
        GPIOB_PCOR |= (1 << GREEN_LED_PIN);  // Seteaza pinul
    } else {
        GPIOB_PSOR |= (1 << GREEN_LED_PIN);  // Stinge pinul
    }

    // Setare sau stinge LED-ul albastru
    if (blue) {
        GPIOD_PCOR |= (1 << BLUE_LED_PIN);  // Seteaza pinul
    } else {
        GPIOD_PSOR |= (1 << BLUE_LED_PIN);  // Stinge pinul
    }

    // Setare sau stinge LED-ul negru
    if (black) {
        GPIOD_PCOR |= (1 << BLACK_LED_PIN);  // Seteaza pinul
    } else {
        GPIOD_PSOR |= (1 << BLACK_LED_PIN);  // Stinge pinul
    }
}

void Test_Loopback(void)
{
	uint8_t in;
	while (1)
	{
    in = UART0_Receive();
    if (in == 0x4A || in ==0x4E)
    {
        // Red: error, data doesn't match
        Control_RGB_LEDs(1, 0, 0, 0); // Turn on the red LED
    }
    else if (in == 0x42 || in ==0x46)
    {
        Control_RGB_LEDs(0, 0, 1, 0); // Turn on the blue LED
    }
    else if (in == 0xE2 || in == 0xE6)
    {
        Control_RGB_LEDs(0, 1, 0, 0); // Turn on the green LED
    }
		else{
			Control_RGB_LEDs(0, 0, 0, 1); // Turn off leds
		}
    UART0_Transmit(in);
	}
}

