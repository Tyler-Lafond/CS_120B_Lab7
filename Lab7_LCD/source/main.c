/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # 6  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum LED_States { LED_SMStart, LED_Change_Unpress, LED_Pause_Press, LED_Pause_Unpress, LED_Change_Press } LED_State;
unsigned char tempB;
unsigned char direct; // 0x00 for shift left and 0x01 for shift right

void TickLED()
{
	switch(LED_State)
	{
		case LED_SMStart:
			tempB = 0x01;
			direct = 0x00;
			LED_State = LED_Change_Unpress;
			break;
		case LED_Change_Unpress:
			if ((PINA & 0x01) == 0x00)
			{
				LED_State = LED_Pause_Press;
			}
			else if ((PINA & 0x01) == 0x01)
			{
				if (tempB == 0x04)
				{
					tempB = tempB >> 1;
					direct = 0x01;
				}
				else if (tempB == 0x02)
				{
					if(direct)
					{
						tempB = tempB >> 1;
					}
					else
					{
						tempB = tempB << 1;
					}
				}
				else if (tempB == 0x01)
				{
					tempB = tempB << 1;
					direct = 0x00;
				}
				LED_State = LED_Change_Unpress;
			}
			break;
		case LED_Pause_Press:
			if ((PINA & 0x01) == 0x01)
			{
				LED_State = LED_Pause_Unpress;
			}
			else if ((PINA & 0x01) == 0x00)
			{
				LED_State = LED_Pause_Press;
			}
			break;
		case LED_Pause_Unpress:
			if ((PINA & 0x01) == 0x00)
			{
				tempB = 0x01;
				direct = 0x00;
				LED_State = LED_Change_Press;
			}
			else if ((PINA & 0x01) == 0x01)
			{
				LED_State = LED_Pause_Unpress;
			}
			break;
		case LED_Change_Press:
			if ((PINA & 0x01) == 0x01)
			{
				if (tempB == 0x04)
				{
					tempB = tempB >> 1;
					direct = 0x01;
				}
				else if (tempB == 0x02)
				{
					if(direct)
					{
						tempB = tempB >> 1;
					}
					else
					{
						tempB = tempB << 1;
					}
				}
				else if (tempB == 0x01)
				{
					tempB = tempB << 1;
					direct = 0x00;
				}
				LED_State = LED_Change_Unpress;
			}
			else if ((PINA & 0x01) == 0x00)
			{
				if (tempB == 0x04)
				{
					tempB = tempB >> 1;
					direct = 0x01;
				}
				else if (tempB == 0x02)
				{
					if(direct)
					{
						tempB = tempB >> 1;
					}
					else
					{
						tempB = tempB << 1;
					}
				}
				else if (tempB == 0x01)
				{
					tempB = tempB << 1;
					direct = 0x00;
				}
				LED_State = LED_Change_Press;
			}
			break;
		default:
			LED_State = LED_SMStart;
			break;
	}

	switch(LED_State)
	{
		case LED_Change_Unpress:
			PORTB = tempB;
			break;
		case LED_Pause_Press:
			break;
		case LED_Pause_Unpress:
			break;
		case LED_Change_Press:
			PORTB = tempB;
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300);
	TimerOn();
    /* Insert your solution below */
	tempB = 0x00;
    while (1) {
	TickLED();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
