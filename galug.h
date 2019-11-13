#ifndef GALUG_H_
#define GALUG_H_

struct enemy {
	unsigned char x;
	unsigned char y;
	unsigned char rate;
	unsigned char del;
	struct enemy *next;
};

void init_enemy();
struct enemy* new_enemy(unsigned char y, unsigned char rate);
void delete_enemy(struct enemy *LL);
unsigned int Galug_game(int seed);


#endif
