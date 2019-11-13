#ifndef DODGE_H_
#define DODGE_H_

unsigned int Dodge_game(int seed);

struct dodge_enemy {
	unsigned char x;
	unsigned char y;
	unsigned char rate;
};

#endif
