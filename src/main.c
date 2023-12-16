#include "Adc.h"
#include "uart.h"
#include "Pit.h"
#include "gpio.h"
#include "Pwm.h"
#include "ClockSettings.h"
#include <math.h>
#define VREF 3.3  // Example reference voltage in volts
#define SPL_REF 94.0 
int main() {
	//SystemClock_Configure();
  //SystemClockTick_Configure();
	OutputPIN_Init();
	UART0_Initialize(115200);
	ADC0_Init();
	PIT_Init();

  //TPM0_Init();
	for(;;) {
		
		if(flag){
	  	int i;
				char v[8];
				int count = 0;
				int value;

				// Converteste valoarea analoga in decibeli
				float voltage = (float)analog_input;

				// Calculate Sound Pressure Level in dB
				//float dbValue = SPL_REF + 20 * log10(voltage / VREF);

				// Rotunjeste valoarea
				value = (int)voltage;
				
				// Afiseaza valoarea obtinuta
				UART0_Transmit('S');
				UART0_Transmit('N');
				UART0_Transmit('D');
				UART0_Transmit(':');

				for (i = 0; i < 8; i++) {
						v[i] = '-';
				}

				while (value != 0) {
						v[count] = (char)(value % 10) + 0x30;
						value = value / 10;
						count = count + 1;
				}

				for (i = 0; i < count; i++) {
						UART0_Transmit(v[count - i - 1]);
				}
				

				UART0_Transmit(0x0A);
				UART0_Transmit(0x0D);
				
				
				flag=0;
				}
//			if(flag_1s){
//				Signal_Control();
//				flag_1s = 0U;
//			}
		}
	}
	
