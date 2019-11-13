#include "init.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

extern unsigned char player_x;
extern unsigned char disp[16][2];
extern unsigned char mode;
extern unsigned char check;

unsigned char numbers[10][4] = { 	{0b01111111, 0b01000001, 0b01000001, 0b01111111},
																 	{0b01000010, 0b01000001, 0b01111111, 0b01000000},
																 	{0b01111001, 0b01001001, 0b01001001, 0b01001111},
																	{0b01001001, 0b01001001, 0b01001001, 0b01111111},
																	{0b00001000, 0b00001100, 0b00001010, 0b01111111},
																	{0b01001111, 0b01001001, 0b01001001, 0b01111001},
																	{0b01111111, 0b01001001, 0b01001001, 0b01111001},
																	{0b00001111, 0b00000001, 0b00000001, 0b01111111},
																	{0b01111111, 0b01001001, 0b01001001, 0b01111111},
																	{0b01001111, 0b01001001, 0b01001001, 0b01111111}
																};

unsigned char No[2][5] = 	{ {0b01111110, 0b00000100, 0b00011000, 0b00100000, 0b01111110},
 														{0b01110000, 0b01010000, 0b01110000, 0b00000000, 0b00000000}
													};
unsigned char R[4] = { 0b01111111, 0b00011001, 0b00101001, 0b01000110};
void init_process()
{
	curtain();
	check = 0;
	player_x = 1;
	mode = INIT_DISP;
	while(mode == INIT_DISP)
	{
		switch(player_x){
			case 0:
				print_number(1,0,numbers[1],4);
				clear_up(6,0,9);
				break;
			case 1:
				clear_up(1,0,4);
				print_number(6,0,numbers[2],4);
				clear_up(11,0,4);
				break;
			case 2:
				clear_up(1,0,9);
				print_number(11,0,R,4);
				break;
			default:
				break;
		}
	}
	TIMSK &= ~(1 << TOIE1);
	for(int i = (player_x * 5) + 9; i < (player_x * 5) + 13; i++)
	{	
		disp[i][1] = disp[i][0];
		disp[i][0] = 0;
	}
	_delay_ms(800);
	curtain();
	TIMSK |= (1 << TOIE1);
}

void print_number(unsigned int start_line, unsigned int color, unsigned char *n_line, unsigned int line_num) {
	unsigned int end_line;

	if(start_line < 0)
		start_line = 8;
	else
		start_line += 8;

	end_line = start_line + line_num;
	for(unsigned int i = 0; start_line< end_line && start_line < 24; i++, start_line++)
		disp[start_line][color] = n_line[i];
}
void clear_up(unsigned int start_line, unsigned int color, unsigned int line_num) {
	unsigned int end_line;

	if(start_line < 0)
		start_line = 8;
	else
		start_line += 8;

	end_line = start_line + line_num;
	for(unsigned int i = 0; start_line< end_line && start_line < 24; i++, start_line++)
		disp[start_line][color] = 0;
}  
void start_count()
{
	TIMSK &= ~(1 << TOIE1);
	clear_up(0,0,16);
	clear_up(0,1,16);
	for(int i = 15; i >= 6; i--)
	{
		clear_up(i+4,0,1);
		print_number(i,0,numbers[3],4);
		_delay_ms(40);
	}
	_delay_ms(600);
	for(int i = -3; i <= 6; i++)
	{
		clear_up(i - 1,0,1);
		print_number(i,0,numbers[2],4);
		_delay_ms(40);
	}
	_delay_ms(600);
	for(int i = 15; i >= 6; i--)
	{
		clear_up(i+4,0,1);
		print_number(i,0,numbers[1],4);
		_delay_ms(40);
	}
	_delay_ms(600);
	clear_up(6,0,4);
	TIMSK |= (1 << TOIE1);
}

void print_score(unsigned int score)
{
	TIMSK &= ~(1 << TOIE1);
	unsigned char p_score[3];
	for(int i = 2; i >= 0; i--)
	{
		p_score[i] = score % 10;
		score /= 10;
	}
	
	clear_up(0,0,16);
	clear_up(0,1,16);
	print_number(1,0,numbers[p_score[0]],4);
	print_number(6,0,numbers[p_score[1]],4);
	print_number(11,0,numbers[p_score[2]],4);
	_delay_ms(700);
	clear_up(1,0,14);
	_delay_ms(300);
	print_number(1,0,numbers[p_score[0]],4);
	print_number(6,0,numbers[p_score[1]],4);
	print_number(11,0,numbers[p_score[2]],4);
	_delay_ms(700);
	clear_up(1,0,14);
	_delay_ms(300);
	print_number(1,0,numbers[p_score[0]],4);
	print_number(6,0,numbers[p_score[1]],4);
	print_number(11,0,numbers[p_score[2]],4);
	_delay_ms(2000);
	clear_up(1,0,14);
	TIMSK |= (1 << TOIE1);
} 
void print_record(unsigned char rank)
{
	TIMSK &= ~(1 << TOIE1);
	clear_up(0,0,16);
	clear_up(0,1,16);
	for(signed int i = 14; i>=1; i--)
	{
		print_number(i,0,No[0],5);
		clear_up(i + 5,0,1);
		print_number(i + 6,0,No[1],3);
		clear_up(i + 9,0,1);
		print_number(i + 10,1,numbers[rank],4);
		clear_up(i + 14,1,1);
		_delay_ms(50);
	}
	_delay_ms(1000);
	clear_up(1,0,9);
	clear_up(11,1,4);
	_delay_ms(400);
	print_number(1,0,No[0],5);
	print_number(7,0,No[1],5);
	print_number(11,1,numbers[rank],4);
	_delay_ms(1000);
	clear_up(1,0,9);
	clear_up(11,1,4);
	TIMSK |= (1 << TOIE1);
}
void curtain()
{
	TIMSK &= ~(1 << TOIE1);
	for(int i = 8; i <= 15; i++)
 	{
 		disp[i][0] = 0xFF;
		disp[i][1] = 0x00;
		disp[31 - i][0] = 0xFF;
		disp[31 - i][1] = 0x00;
		_delay_ms(50);
 	}
	_delay_ms(400);
	for(int i = 15; i >= 8; i--)
 	{
 		disp[i][0] = 0x00;
		disp[31 - i][0] = 0x00;
		_delay_ms(50);
 	}
	_delay_ms(400);
	TIMSK |= (1 << TOIE1);
}
