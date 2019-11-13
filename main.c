#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "Button.h"
#include "init.h"
#include "galug.h"
#include "dodge.h"
#include "Score.h"

extern unsigned char alpha[26][5];

volatile unsigned char disp[24][2] = {{0, },};
volatile unsigned char disp_y = 8;
volatile unsigned int RG = 0;
volatile unsigned char mode = 0;
volatile unsigned char button_del[6] = { 0, };
unsigned char player_x;
unsigned char player_y;
unsigned char game_flag;
unsigned char skill_flag;
volatile signed int check;
volatile unsigned int Round_check;
/*
ÇÉ¹èÄ¡
PORTA 0~7 = display (-)

PORTC 0~2 = display (+)G
PORTC 3 = left G
PORTC 4 = right G
PORTD 0~2 = display (+)R
PORTD 3 = left R
PORTD 4 = right R

PORTB 0~3 = button 1~4
PORTD 6 = button 5
*/

/*
T/C
2 = display 
1 = button
*/

ISR(TIMER2_OVF_vect)
{
	if(RG & 1)
	{
		PORTD = 0x00;
		PORTC = disp_y;
		PORTA = disp[disp_y][1];
		if(disp_y >= 23)
			disp_y = 8;
		else
			disp_y++;
	}
	else
	{	
		PORTC = 0x00;
		PORTD = disp_y;
		PORTA = disp[disp_y][0];
	}	
	RG++;
	TCNT2 = 0x95;
}	
ISR(TIMER1_OVF_vect)
{
	Button_Check(arrow_left,0,mode);
	Button_Check(arrow_up,1,mode);
	Button_Check(arrow_right,2,mode);
	Button_Check(arrow_down,3,mode);
	Button_Check(push_button1,4,mode);
	Button_Check(push_button2,5,mode);	

	check++;
	Round_check++;
	TCNT1H = 0xF3;
	TCNT1L = 0xEA;
}								 
int main()
{
	unsigned int score;
	unsigned char rank;
	DDRA = 0xFF;
	DDRB = 0x00;
	DDRC = 0xFF;
	DDRD = 0x1F;
	
	TCCR2 = (1 << CS21) | (1 << CS20);
	TCCR1B = (1 << CS12);
		
	TCNT2 = 0x95;
	TCNT1H = 0xF3;
	TCNT1L = 0xEA;
	
	TIMSK = (1 << TOIE1) | (1 << TOIE2);
	sei();

	while(1)
	{
		init_process();
		switch(mode)
		{
			case GAME_1:
				start_count();
				score = Dodge_game((TCNT1H << 8) + TCNT1L);
				TIMSK &= ~(1 << TOIE1);
				_delay_ms(1000);
				TIMSK |= (1 << TOIE1);
				curtain();
				print_score(score);
				if((rank = is_record(score, GAME_1)))
				{	
					print_record(rank);
					mode = RESISTER_NAME;
					register_rank(score, rank, GAME_1);
				}
				break;
			case GAME_2:
				start_count();
				score = Galug_game((TCNT1H << 8) + TCNT1L);
				TIMSK &= ~(1 << TOIE1);
				_delay_ms(1000);
				TIMSK |= (1 << TOIE1);
				curtain();
				print_score(score);
				if((rank = is_record(score, GAME_2)))
				{	
					print_record(rank);
					mode = RESISTER_NAME;
					register_rank(score, rank, GAME_2);
				}	
				break;
			case SCORE_BOARD:
				print_rank();
				break;
			default:
				break;
		}
	}

	return 0;
}

