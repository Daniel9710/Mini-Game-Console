#include "init.h"
#include"Button.h"

extern unsigned char disp[24][2];
extern unsigned char mode;
extern unsigned char player_x;
extern unsigned char player_y;
extern unsigned char skill_flag;
extern signed int check;
extern unsigned char wall;
extern unsigned char bullet_flag;
extern signed int bullet_num;
extern unsigned char rank_flag;
extern unsigned char game_flag;
extern unsigned char change_flag;
extern unsigned char alpha_flag[3];
extern unsigned char score_alpha;
/*arrow_left*/
void Button_Event_0(int cs)
{
	switch(cs)
	{
		case INIT_DISP:
			if(player_x > 0)
				player_x--;
			break;

		case GAME_1:
			if(player_x > (8 + wall))
			{
				disp[player_x][1] &=~player_y;
				player_x--;
				disp[player_x][1] |= player_y;
				if(skill_flag)
				{
					disp[player_x + 1][0] &= ~player_y;
					disp[player_x][0] |= player_y;
				}
			}
			break;

		case SCORE_BOARD:
		case RESISTER_NAME:
			if((rank_flag % 3) > 0)
			{
				rank_flag--;
				change_flag = 1;
			}
			break;
		default:
			break;
	}
}
/*arrow_up*/
void Button_Event_1(int cs)
{
	switch(cs)
	{
		case GAME_1:
			if(player_y > 0x01)
			{
				disp[player_x][1] &= ~player_y;
				player_y >>= 1;
				disp[player_x][1] |= player_y;
				if(skill_flag)
				{
					disp[player_x][0] &= ~(player_y << 1);
					disp[player_x][0] |= player_y;
				}
			}
			break;

		case GAME_2:
			if(player_y > 0x01)
			{
				player_y >>= 1;
				disp[player_x][1] = player_y;
				if(skill_flag)
				{
					disp[player_x][0] &= ~(player_y << 1);
					disp[player_x][0] |= player_y;
				}
			}
			break;

		case SCORE_BOARD:
			if(rank_flag >= 3)
			{
				rank_flag -= 3;
				change_flag = 1;
			}
			break;
		case RESISTER_NAME:
			if(alpha_flag[rank_flag] >= 25)
				alpha_flag[rank_flag] = 0;
			else 
				alpha_flag[rank_flag]++;
			change_flag = 1;
			break;
		default:
			break;
	}
}
/*arrow_right*/
void Button_Event_2(int cs)
{
	switch(cs)
	{
		case INIT_DISP:
			if(player_x < 2)
				player_x++;
			break;

		case GAME_1:
			if(player_x < (23 - wall))
			{
				disp[player_x][1] &= ~player_y;
				player_x++;
				disp[player_x][1] |= player_y;
				if(skill_flag)
				{
					disp[player_x - 1][0] &= ~player_y;
					disp[player_x][0] |= player_y;
				}
			}
			break;

		case SCORE_BOARD:
		case RESISTER_NAME:
			if((rank_flag % 3) < 2)
			{
				rank_flag++;
				change_flag = 1;
			}
			break;
		default:
			break;
	}
}
/*arrow_down*/
void Button_Event_3(int cs)
{
	switch(cs)
	{
		case GAME_1:
			if(player_y < 0x80)
			{
				disp[player_x][1] &= ~player_y;
				player_y <<= 1;
				disp[player_x][1] |= player_y;
				if(skill_flag)
				{
					disp[player_x][0] &= ~(player_y >> 1);
					disp[player_x][0] |= player_y;
				}
			}
			break;

		case GAME_2:
			if(player_y < 0x80)
			{
				player_y <<= 1;
				disp[player_x][1] = player_y;
				if(skill_flag)
				{
					disp[player_x][0] &= ~(player_y >> 1);
					disp[player_x][0] |= player_y;
				}
			}
			break;

		case SCORE_BOARD:
			if(rank_flag < 3)
			{
				rank_flag += 3;
				change_flag = 1;		
			}
			break;
		case RESISTER_NAME:
			if(alpha_flag[rank_flag] <= 0)
				alpha_flag[rank_flag] = 25;
			else
				alpha_flag[rank_flag]--;
			change_flag = 1;
			break;
		default:
			break;
	}
}
/*push_button1*/
void Button_Event_4(int cs)
{
	switch(cs)
	{
		case INIT_DISP:
			mode = 2 + player_x;
			break;

		case GAME_1:
			if(skill_flag)
			{
				check = -150;
				skill_flag = 0;
			}
			break;

		case GAME_2:	
			if(bullet_num >=0)
			{
				bullet_flag = 1;
				bullet_num++;
				if(bullet_num >= 10)
					bullet_num = -8;
			}
			break;

		case SCORE_BOARD:
		case RESISTER_NAME:
			game_flag = 0;			
			change_flag = 1;
			break;

		default:
			break;
	}
}
/*push_button2*/
void Button_Event_5(int cs)
{
	switch(cs)
	{
		case GAME_1:
			if(skill_flag && (wall > 0))
			{
				wall--;
				disp[8 + wall][1] = 0x00;
				disp[23 - wall][1] = 0x00;
				disp[8 + wall][0] = 0x00;
				disp[23 - wall][0] = 0x00;
				skill_flag = 0;
			}
			break;

		case GAME_2:
			if(skill_flag)
				bullet_flag = 9;
				skill_flag = 0;
			break;
		case SCORE_BOARD:
			score_alpha++;
			change_flag = 1;
			break;
		default:
			break;
	}
}
