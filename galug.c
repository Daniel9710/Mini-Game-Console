#include<stdlib.h>
#include<avr/interrupt.h>
#include "galug.h"
#include "init.h"

#define POOL_SIZE 70

extern unsigned char disp[16][2];
extern unsigned char player_x;
extern unsigned char player_y;
extern unsigned char game_flag;
extern unsigned char skill_flag;
extern signed int check;
extern unsigned int Round_check;

volatile signed int bullet_num;
volatile unsigned char bullet_flag;
struct enemy enemys[POOL_SIZE];
struct enemy *top = enemys;

void init_enemy()
{
  struct enemy *r=enemys;
  struct enemy *s;

  enemys[POOL_SIZE - 1].next=NULL;  

  for(int i = 1; i < POOL_SIZE; i++) { 
    s = r++;
    s->next = r;
  }
}

struct enemy* new_enemy(unsigned char y, unsigned char rate)
{	
	struct enemy *node = top;
	if(top == NULL)
		return NULL;

	top = top->next;
	node->x = 23;
	node->y = (1 << y);
	node->rate = rate;
	node->del = 0;
	node->next = NULL;
	return node;
}

unsigned int Galug_game(int seed)
{
	struct enemy *LL;
	struct enemy *tmp;
	unsigned char tmp_x;
	unsigned char tmp_y;
	int i;
	unsigned int score = 0;
	unsigned char round_rate = 27;
	unsigned char rate_flag = 1;
	srand(seed);

	player_x = 10;
	player_y = 0x08;
	disp[player_x][1] = player_y;
	disp[8][1] = 0xFF;
	game_flag = 1;
	init_enemy();
	LL = NULL;
	Round_check = 0;
	check = 0;
	skill_flag = 0;
	bullet_num = 0;
	bullet_flag = 0;
	
	while(1)
	{
		if(check >= 1)
		{
			if((Round_check % round_rate) == 0)
			{	
				unsigned char index = rand();
				tmp = new_enemy(index % 8, 5 - (index % rate_flag));
				if(tmp == NULL)
					continue;
				tmp->next = LL;
				LL = tmp;
				
			}
			if((Round_check % 800) == 0)
			{
				skill_flag = 1;
				if(round_rate > 9)
					round_rate -= 3;
				if(rate_flag < 4);
					rate_flag++;
			}
			tmp = LL;
			while(tmp)
			{
				if(tmp->rate <= tmp->del)
				{
					(tmp->x)--;
					tmp->del = 0;
				}
				else
					(tmp->del)++;
				tmp = tmp->next;
			}
			clear_up(0,0,16);
				
			tmp = LL;
			while(tmp)
			{
				tmp_x = tmp->x;
				tmp_y = tmp->y;
				disp[tmp_x][0] |= tmp_y;
				
				if((disp[tmp_x][0] & tmp_y) && (disp[tmp_x][1] & tmp_y))
				{	
					struct enemy *r = tmp->next;
					disp[tmp_x][0] &= ~tmp_y;
					if(tmp_x != 10)
					{	
						disp[tmp_x][1] &= ~tmp_y;
						score += 3;
					}
					if(tmp == LL)
						LL = LL->next;
					else
					{
						struct enemy *p = LL;
						while(p->next != tmp)
							p = p->next;
						p->next = tmp->next;
					}
					tmp->next = top;
					top = tmp;
					tmp = r;
				}
				else
					tmp = tmp->next;
			}
			clear_up(0,0,16);
			tmp = LL;
			while(tmp)
			{
				disp[tmp->x][0] |= tmp->y;
				tmp = tmp->next;
			}
			if(skill_flag)
			{
				disp[player_x][0] |= player_y;
			}
			i = disp[23][1];
			for(i = 23; i > 11; i--)
				disp[i][1] = disp[i - 1][1];
			
			if(bullet_flag > 1)
			{
				disp[11][1] = 0xFF;	
				bullet_flag--;
				bullet_num = 0;
			}
			else if(bullet_flag == 1)
			{
				disp[11][1] = player_y;
				bullet_flag = 0;
			}
			else
			{
				disp[11][1] = 0;
				bullet_flag = 0;
				if(bullet_num < 0)
					bullet_num++;		
			}
			tmp = LL;
			while(tmp)
			{
				tmp_x = tmp->x;
				tmp_y = tmp->y;
				
				if((disp[tmp_x][0] & tmp_y) && (disp[tmp_x][1] & tmp_y))
				{	
					struct enemy *r = tmp->next;
					disp[tmp_x][0] &= ~tmp_y;
					if(tmp_x != 10)
					{	
						disp[tmp_x][1] &= ~tmp_y;
						score += 5;
					}
					if(tmp == LL)
						LL = LL->next;
					else
					{
						struct enemy *p = LL;
						while(p->next != tmp)
							p = p->next;
						p->next = tmp->next;
					}
					tmp->next = top;
					top = tmp;
					tmp = r;
				}
				else
					tmp = tmp->next;
			}
			if(disp[8][0])
				break;

			check = 0;
		}
	}
	if(score > 999)
		score = 999;
	return score;
}
	
