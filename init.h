#ifndef INIT_H_
#define INIT_H_

#define INIT_DISP 0
#define GAME_1 2
#define GAME_2 3
#define SCORE_BOARD 4
#define RESISTER_NAME 5

void init_process();
void print_number(unsigned int start_line, unsigned int color, unsigned char *n_line, unsigned int line_num);
void clear_up(unsigned int start_line, unsigned int color, unsigned int line_num);
void start_count();
void print_score(unsigned int score);
void print_record(unsigned char rank);
void curtain();
#endif
