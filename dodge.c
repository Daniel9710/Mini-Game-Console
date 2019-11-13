#include <stdlib.h>
#include "dodge.h"
#include "init.h"

extern unsigned char disp[16][2];
extern unsigned char player_x;
extern unsigned char player_y;
extern unsigned char game_flag;
extern unsigned char skill_flag;
extern signed int check;
extern unsigned int Round_check;

volatile unsigned char wall;

unsigned int Dodge_game(int seed)
{
	unsigned char enemy_num = 0;
	unsigned char enemy_x;
	unsigned char enemy_y;
	struct dodge_enemy enemy[50];
	unsigned char enemy_del[50];
	signed int i;
	
	srand(seed);

	player_x = 16;
	player_y = 0x08;
	disp[player_x][1] = player_y;
	
	game_flag = 1;

	for(i = 0; i< 50; i++)
		enemy_del[i] = 0;
	check = 0;
	Round_check = 0;
	wall = 0;
	skill_flag = 0;
	while(game_flag)
	{
		if(check >= 4)
		{ 
			if((Round_check % 172) == 0)
			{
				int tmp = rand();
				enemy[enemy_num].rate = 10 - (Round_check / 1000);
				enemy[enemy_num].x = (tmp % (16 - (2 * wall))) + 8 + wall;
				if(tmp & 1)
					enemy[enemy_num].y = 0x80;
				else
					enemy[enemy_num].y = 0x01;
				enemy_num++;
			}
			if((Round_check % 800) == 0)
			{
				disp[8 + wall][0] = 0xFF;
				disp[8 + wall][1] = 0xFF;
				disp[23 - wall][0] = 0xFF;
				disp[23 - wall][1] = 0xFF;
				wall++;
				if(player_x < 8 + wall)
				{
					player_x++;
					disp[player_x][1] |= player_y;
				}
				else if(player_x > 23 - wall)
				{
					player_x--;
					disp[player_x][1] |= player_y;
				}
				skill_flag = 1;
			}
			for(i = 0; i< enemy_num;i++)
			{
				if(enemy[i].rate <= enemy_del[i])
				{
					enemy_x = enemy[i].x;
					enemy_y = enemy[i].y;
					if(enemy_x < player_x)
						enemy[i].x++;
					else if(enemy_x > player_x)
						enemy[i].x--;
					if(enemy_y < player_y)
						enemy[i].y <<= 1;
					else if(enemy_y > player_y)
						enemy[i].y >>= 1;
					enemy_del[i] = 0;
				}
				else
					enemy_del[i]++;
			}
			clear_up(wall,0,16 - 2 * wall);
			for(i = 0; i < enemy_num;i++)
				disp[enemy[i].x][0] |= enemy[i].y;
			if(skill_flag)
				disp[player_x][0] |= player_y;
			check = 0;
		}
		if(check >= 0)
		{
			for(i = 0; i < enemy_num; i++)
			{
				if(enemy[i].x == player_x && enemy[i].y == player_y)
				{
					game_flag = 0;
					break;
				}
			}
		}
	}
	if(Round_check > 9990)
		Round_check = 9990;
	return (Round_check / 10);
}
