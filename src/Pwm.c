#include "Pwm.h"

#define SERVO_PIN (2) // PORT B, PIN 2

void TPM2_Init(){
	
	// Activarea semnalului de ceas pentru utilizarea LED-ului de culoare rosie
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Utilizarea alternativei de functionare pentru perifericul TMP
	// TMP2_CH0
	PORTB->PCR[SERVO_PIN] |= PORT_PCR_MUX(3);
	

	// Selects the clock source for the TPM counter clock (MCGFLLCLK) - PG. 196
	// MCGFLLCLK Freq. - 20 MHz
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// Activarea semnalului de ceas pentru modulul TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM2(1);
	
	// Divizor de frecventa pentru ceasul de intrare
	// PWM CLK -> 48MHz / 128 = 48.000.000 / 128 [Hz] = 375.000 [Hz] = 375 kHz
	TPM2->SC |= TPM_SC_PS(7);
	
		// LPTPM counter operates in up counting mode. - PG. 553
	// Selects edge aligned PWM
	TPM2->SC |= TPM_SC_CPWMS(0);
	
	// LPTPM counter increments on every LPTPM counter clock
	TPM2->SC |= TPM_SC_CMOD(1);
	
	
	// LPTPM counter operates in up-down counting mode. - PG. 553
	// Selects center aligned PWM
	//TPM2->SC |= TPM_SC_CPWMS(1);
	
	
	// Edge-Aligned Pulse Width Modulation
	// TPM->SC[CPWMS] = 0
	
	// ===== Mode selection ====
	// Configured for EPWM
	// TPM->CnSC[MnSB] = 1
	// TPM->CnSC[MnSB] = 0
	
	// ==== Edge selection ====
	// Set output on counter overflow, clear output on match
	// Counter overflow = LPTPM counter reaches the value configured in the MOD register, and then resets
	// Match = LPTPM counter reaches the value configured in the CnV register
	
	// TPM->CnSC[ELSnB] = 1
	// TPM->CnSC[ELSnA] = 0 
	
	// Regiter associated to the control of channel 0
	// Why channel 0?
	TPM2->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
	
	
}

void Signal_Control(uint8_t position){
    
	// Resetarea valorii numaratorului asociat LPTPM Counter
	   TPM2->MOD = 7500;  // 375,000 Hz / 7500 = 50 Hz (20 ms)

    // Setarea duty cycle-ului pentru 3 pozitii
    switch (position) {
        case 0:
            TPM2->CONTROLS[0].CnV = 188;  // 2.5% pentru 0�
            break;
        case 1:
            TPM2->CONTROLS[0].CnV = 562;  // 7.5% pentru 90�
            break;
        case 2:
            TPM2->CONTROLS[0].CnV = 937;  // 12.5% pentru 180�
            break;
    }
}
