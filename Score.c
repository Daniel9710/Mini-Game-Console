#include <avr/eeprom.h>
#include <avr/delay.h>
#include "init.h"

extern unsigned char game_flag;
extern unsigned char disp[16][2];
extern unsigned char numbers[10][4];
extern signed int check;

unsigned char alpha[26][5] = { {0b01111110, 0b00001001, 0b00001001, 0b00001001, 0b01111110},
															 {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110},
															 {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b01000001},
															 {0b01111111, 0b01000001, 0b01000001, 0b01000001, 0b00111110},
															 {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b01001001},
															 {0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00001001},
															 {0b00111110, 0b01000001, 0b01001001, 0b01001001, 0b00111001},
															 {0b01111111, 0b00001000, 0b00001000, 0b00001000, 0b01111111},
															 {0b00000000, 0b01000001, 0b01111111, 0b01000001, 0b00000000},
															 {0b01110000, 0b01000001, 0b01000001, 0b01111111, 0b00000001},
															 {0b01111111, 0b00001000, 0b00010100, 0b00100010, 0b01000001},
															 {0b01111111, 0b01000000, 0b01000000, 0b01000000, 0b01000000},
															 {0b01111111, 0b00000010, 0b00000100, 0b00000010, 0b01111111},
															 {0b01111111, 0b00000100, 0b00001000, 0b00010000, 0b01111111},
															 {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110},
															 {0b01111111, 0b00001001, 0b00001001, 0b01001001, 0b00000110},
															 {0b00111111, 0b00100001, 0b01100001, 0b00100001, 0b00111111},
															 {0b01111111, 0b00001001, 0b00011001, 0b00101001, 0b01000110},
															 {0b00100110, 0b01001001, 0b01001001, 0b01001001, 0b00110010},
															 {0b00000001, 0b00000001, 0b01111111, 0b00000001, 0b00000001},
															 {0b00111111, 0b01000000, 0b01000000, 0b01000000, 0b00111111},
															 {0b00011111, 0b00100000, 0b01000000, 0b00100000, 0b00011111},
															 {0b00111111, 0b01000000, 0b00111000, 0b01000000, 0b00111111},
															 {0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011},
															 {0b00000111, 0b00001000, 0b01110000, 0b00001000, 0b00000111},
															 {0b01100001, 0b01010001, 0b01001001, 0b01000101, 0b01000011}
														 };

volatile unsigned char rank_flag;
volatile unsigned char change_flag;
volatile unsigned char alpha_flag[3];
volatile unsigned char score_alpha;

unsigned char is_record(unsigned int score, unsigned char mode)
{
	if(score > eeprom_read_word((unsigned int*)(0 + (6 * (mode - 2)))))
		return 1;
	else if(score > eeprom_read_word((unsigned int*)(2 + (6 * (mode - 2)))))
		return 2;
	else if(score > eeprom_read_word((unsigned int*)(4 + (6 * (mode - 2)))))
		return 3;

	return 0;
}
void print_rank()
{
	unsigned char color;
	unsigned char row;
	unsigned int score = 0;
	unsigned int name[3];
	rank_flag = 0;
	game_flag = 1;
	clear_up(1, 0, 14);
	clear_up(1, 1, 14);
	change_flag = 0;
	score_alpha = 0;
	while(game_flag)
	{
		color = rank_flag / 3;
		row = (rank_flag % 3) * 5;
		
		if((score_alpha & 1) == 0)
		{
			score = eeprom_read_word(rank_flag * 2);
			for(signed int i = 18; i >= 8; i-=5)
			{
				disp[i + 1][0] |= numbers[score % 10][0];
				disp[i + 2][0] |= numbers[score % 10][1];
				disp[i + 3][0] |= numbers[score % 10][2];
				disp[i + 4][0] |= numbers[score % 10][3];
				score /= 10;
			}
		}
		else
		{
			name[0] = eeprom_read_byte(15 + (3 * rank_flag));
			name[1] = eeprom_read_byte(16 + (3 * rank_flag));
			name[2] = eeprom_read_byte(17 + (3 * rank_flag));

			for(signed int i = 0; i < 3; i++)
			{
		  	disp[9 + (i * 5)][0] |= alpha[name[i]][0];
				disp[10 + (i * 5)][0] |= alpha[name[i]][1];
				disp[11 + (i * 5)][0] |= alpha[name[i]][2];
				disp[12 + (i * 5)][0] |= alpha[name[i]][3];
				disp[13 + (i * 5)][0] |= alpha[name[i]][4];
			}
		}
		disp[row + 9][color] |= 0x80;
		disp[row + 10][color] |= 0x80;
		disp[row + 11][color] |= 0x80;
		disp[row + 12][color] |= 0x80;
		while(game_flag && change_flag == 0);
		clear_up(0,0,16);
		clear_up(0,1,16);
		change_flag = 0;
	}
}
void register_rank(unsigned int score, unsigned char rank, unsigned char mode)
{
	unsigned char tmp;
	rank_flag = 0;
	alpha_flag[0] = 0;
	alpha_flag[1] = 0;
	alpha_flag[2] = 0;
	game_flag = 1;
	disp[23][0] = 0;
	while(game_flag)
	{
		print_number(1,0,alpha[alpha_flag[0]],5);
		print_number(6,0,alpha[alpha_flag[1]],5);
		print_number(11,0,alpha[alpha_flag[2]],5);
		check = 0;
		while(game_flag && change_flag	== 0)
		{
			if(check == 0)
				print_number((rank_flag * 5) + 1, 0, alpha[alpha_flag[rank_flag]],5);
			else if(check == 10)
				clear_up(rank_flag * 5 + 1, 0, 5);
			else if(check == 20)
				check = 0;
		}
		change_flag = 0;
		
	}
	clear_up(0,0,16);
	TIMSK &= ~(1 << TOIE1);
	print_number(1,1,alpha[alpha_flag[0]],5);
	print_number(6,1,alpha[alpha_flag[1]],5);
	print_number(11,1,alpha[alpha_flag[2]],5);
	_delay_ms(1000);
	clear_up(0,1,16);
  tmp = 6 * (mode - 2);
	switch(rank)
	{
		case 1:
			eeprom_update_word((unsigned int*)(4 + tmp), eeprom_read_word((2 + tmp)));
			eeprom_update_word((unsigned int*)(2 + tmp), eeprom_read_word((0 + tmp)));
			eeprom_update_word((unsigned int*)(0 + tmp), score);
			break;

		case 2:
			eeprom_update_word((unsigned int*)(4 + tmp), eeprom_read_word((2 + tmp)));
			eeprom_update_word((unsigned int*)(2 + tmp), score);
			break;

		case 3:
			eeprom_update_word((unsigned int*)(4 + tmp), score);
			break;
		
		default:
			break;
	}
	tmp = 9 * (mode - 2);
	switch(rank)
	{
		case 1:
			eeprom_update_byte((unsigned char*)(21 + tmp), eeprom_read_byte((18 + tmp)));
			eeprom_update_byte((unsigned char*)(22 + tmp), eeprom_read_byte((19 + tmp)));
			eeprom_update_byte((unsigned char*)(23 + tmp), eeprom_read_byte((20 + tmp))); 
			eeprom_update_byte((unsigned char*)(18 + tmp), eeprom_read_byte((15 + tmp)));
			eeprom_update_byte((unsigned char*)(19 + tmp), eeprom_read_byte((16 + tmp)));
			eeprom_update_byte((unsigned char*)(20 + tmp), eeprom_read_byte((17 + tmp))); 
			eeprom_update_byte((unsigned char*)(15 + tmp), alpha_flag[0]);
			eeprom_update_byte((unsigned char*)(16 + tmp), alpha_flag[1]);
			eeprom_update_byte((unsigned char*)(17 + tmp), alpha_flag[2]); 
			break;

		case 2:
			eeprom_update_byte((unsigned char*)(21 + tmp), eeprom_read_byte((18 + tmp)));
			eeprom_update_byte((unsigned char*)(22 + tmp), eeprom_read_byte((19 + tmp)));
			eeprom_update_byte((unsigned char*)(23 + tmp), eeprom_read_byte((20 + tmp))); 
			eeprom_update_byte((unsigned char*)(18 + tmp), alpha_flag[0]);
			eeprom_update_byte((unsigned char*)(17 + tmp), alpha_flag[1]);
			eeprom_update_byte((unsigned char*)(16 + tmp), alpha_flag[2]); 
			break;

		case 3:
			eeprom_update_byte((unsigned char*)(21 + tmp), alpha_flag[0]);
			eeprom_update_byte((unsigned char*)(22 + tmp), alpha_flag[1]);
			eeprom_update_byte((unsigned char*)(23 + tmp), alpha_flag[2]); 
			break;
		
		default:
			break;
	}
	TIMSK |= (1 << TOIE1);
}
