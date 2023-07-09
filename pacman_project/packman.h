//#ifndef TEST_CONSOLE_PACKMAN_H
//#define TEST_CONSOLE_PACKMAN_H
//
//#include <stdio.h>
//#include <assert.h>
//#include "console.h"
//#include "console.c"
//#include <stdlib.h>
//#include <time.h>
//#include <stdbool.h>
//
//#define TITLE_X 3
//#define TITLE_Y 1
//#define FIELD_PADDING 3
//
//#define CHAR_BORDER '#'
//#define CHAR_FIELD  ' '
//#define CHAR_POINT  '@'
//#define CHAR_FOOD   '*'
//#define CHAR_BOT1    '%'
//
//#define COLOR_BORDER 1
//#define COLOR_FIELD  2
//#define COLOR_POINT  3
//#define COLOR_BOT   4
//
//#define MOVE_TIME_STEP (CLOCKS_PER_SEC / 5)
//
//typedef enum {
//    UP, DOWN, LEFT, RIGHT, NONE
//} Direction;
//
//typedef struct hero{
//    int pointx;
//    int pointy;
//    char symb;
//    char colour;
//    Direction dir;
//    int* possible_moves;
//} hero;
//
//static int field_x, field_y; // top-left corner
//static int field_width, field_height;
//static char *filename = "C:\\Users\\kater\\CLionProjects\\test_console\\map.txt";
//static char *meta_file = "C:\\Users\\kater\\CLionProjects\\test_console\\map_meta.txt";
//static bool game_started;
//static int score = 0;
//static int max_score;
//
//hero *create_bot(int pointx, int pointy, char symb, char colour);
//char *readingfile(char *file_name);
//char **read_map();
//static void con_charAt(int ch, int color, int x, int y);
//static void init_colors();
//void draw_hero(hero *bot);
//static void initial_draw(char **map, hero* main_hero, hero* bot1);
//int process_key(int key, hero* main_hero);
//void move(char **map, hero* main_hero);
//bool legal_move_bot(char **map, int new_x, int new_y);
//void move_bot(char **map, hero *bot, int new_x, int new_y);
//void Possible_moves(char **map, hero *bot);
//void random_moving(hero* bot);
//int check_collision(hero* player, hero* bot1);
//void finish(hero* player, hero* bot1);
//void delete_bot(hero* bot);
//void print_bot(hero* bot);
//
//
//#endif //TEST_CONSOLE_PACKMAN_H
