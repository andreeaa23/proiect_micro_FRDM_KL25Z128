#include "gpio.h"
#include "uart.h"

#define RED_LED_PIN (18) // PORT B
#define GREEN_LED_PIN (19) // PORT B
#define BLUE_LED_PIN (1) // PORT D
#define BLACK_LED_PIN (2) //PORT D


uint8_t ledState = 0;

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
	
	// Initialize PIT
    InitPIT();

 
}

void InitPIT(void) {
    // Enable clock for PIT
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // Enable PIT Module Control Register
    PIT->MCR = 0;

    // Configure PIT channel 0
	  //Timer Load Value Register
    PIT->CHANNEL[0].LDVAL = 11900000;  // Set the load value for 1190 ms
	
	  //Timer Control Register
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK;  // Enable interrupt
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;  // Enable timer

    // Enable PIT interrupt in NVIC
    NVIC_EnableIRQ(PIT_IRQn);
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

void PIT_IRQHandler(void) {
    // Clear interrupt flag
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

// Change LED color after 1190 ms
    switch (ledState) {
        case 0: // Red
            Control_RGB_LEDs(1, 0, 0,0);
            break;
        case 1: // Green
            Control_RGB_LEDs(0, 1, 0,0);
            break;
        case 2: // Blue
            Control_RGB_LEDs(0, 0, 1,0);
            break;
				case 3: //Black
					 Control_RGB_LEDs(0, 0, 0,1);
						break;
        default: // Turn off LEDs
            Control_RGB_LEDs(0, 0, 0,0);
            break;
    }

    // Increment LED state
    ledState++;

    // Check for overflow
    if (ledState > 3) {
        ledState = 0;
    }
}

