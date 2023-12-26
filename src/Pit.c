#include "Pit.h"
#include "gpio.h"
#include "Pwm.h"
uint8_t ledState = 0;
void PIT_Init(void) {
	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 1,119 secunde
	PIT->CHANNEL[1].LDVAL = 56879999;
	//juma de secuda pentru adc
	PIT->CHANNEL[0].LDVAL = 23999999;
	
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	// Activarea întreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		
	}
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK){
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	switch (ledState) {
        case 0: // Red
            Control_RGB_LEDs(1, 0, 0);

            break;
        case 1: // Green
            Control_RGB_LEDs(0, 1, 0);

            break;
        case 2: // Blue
            Control_RGB_LEDs(0, 0, 1);

            break;
        default: // Turn off LEDs
            Control_RGB_LEDs(0, 0, 0);
            break;
    }
		
    // Increment LED state
    ledState++;

    // Check for overflow
    if (ledState > 3) {
        ledState = 0;
    }
	}
	
	
	
}