#include "MKL25Z4.h"

void OutputPIN_Init(void);
void Control_RGB_LEDs(uint8_t red, uint8_t green, uint8_t blue);
void Test_Loopback(void);
void PIT_IRQHandler2(void);
void InitPIT(void);