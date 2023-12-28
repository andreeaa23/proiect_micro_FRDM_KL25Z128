#include "ClockSettings.h"
#include "Pwm.h"
#include "Pit.h"
#include "adc.h"
#include "gpio.h"
#include "uart.h"


int main() {
	
	SystemClock_Configure();
	SystemClockTick_Configure();
	OutputPIN_Init();
	TPM2_Init();
	UART0_Init(115200);
	ADC0_Init();
	PIT_Init();
	
	for(;;){
		if(flag){
			
	  	  int i;
				char v[8];
				int count = 0;
				int value;

				value = (int)analog_input;
				
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
		
		if(flag_1s){
			if(analog_input<23){
				Signal_Control(0);
			}
			else if(analog_input>=23&analog_input<46){
				Signal_Control(1);
			}
			else if(analog_input>46){
				Signal_Control(2);
			}
			flag_1s = 0U;
		}
	}
}
