#include <stdio.h>
#include <assert.h>
#include "console.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define TITLE_X 3
#define TITLE_Y 1
#define FIELD_PADDING 3

#define CHAR_BORDER '#'
#define CHAR_FIELD  ' '
#define CHAR_POINT  '@'
#define CHAR_FOOD   '*'
#define CHAR_BOT1    '%'
#define CHAR_BOT2   '$'
#define CHAR_BOT3   '&'

#define COLOR_BORDER 1
#define COLOR_FIELD  2
#define COLOR_POINT  3
#define COLOR_BOT   4

#define MOVE_TIME_STEP (CLOCKS_PER_SEC / 5)

static int field_x, field_y; // верхний левый угол
static int field_width, field_height; // ширина и длина карты
static char *filename = "D:\\map.txt"; //файл, в котором должна лежать карта в виде символов
static char *meta_file = "D:\\map_meta.txt"; //файл с доп. данными( длина и ширина поля, максимальное кол-во еды
static bool game_started;
static int score = 0;
static int max_score;
static int bot1_x, bot1_y, bot_2x, bot_2y, bot_3x, bot_3y;


///Для тестов. Выводит координаты бота, возможные движения и направление движения.///
void print_hero(hero *bot) {
    printf("%d %d \n", bot->pointx, bot->pointy);
    for (int i = 0; i < 4; i++)
        printf("%d ", bot->possible_moves[i]);
    printf("\n%u", bot->dir);
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Создание героя/бота. Возвращает указатель на него.///
hero *create_hero(int pointx, int pointy, char symb, char colour) {
    hero *hero1 = (hero *) malloc(sizeof(hero));
    if (hero1 == NULL) {
        printf("Memory error!");
        exit(1);
    }
    hero1->pointx = pointx;
    hero1->pointy = pointy;
    hero1->symb = symb;
    hero1->dir = NONE;
    hero1->colour = colour;
    hero1->possible_moves = calloc(4, sizeof(int));
    return hero1;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Чтение карты из файла. Возвращает указатель на строку с содержимым файла.///
char *readingfile(char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "\"%s\" not available.\n", file_name);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    size_t size = sizeof(char) * (length);
    char *contents = malloc(size + 1);
    if (contents == NULL) {
        fprintf(stderr, "Memory error\n");
        return 0;
    }
    fread(contents, size, 1, file);
    fclose(file);
    contents[size] = '\0';
    return contents;
}
//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Читает необходимые параметры из двух файлов:///
///         1) из файла filename получаем карту char** map[][];///
///         2) из файла meta_file получим длину, ширину карты и максимальное кол-во еды. Все три параметра должны быть записаны через '\n'!///
///             Далее через пробел для каждого бота с новой строчки считываются начальные координаты трёх ботов.///
char **read_map() {
    FILE *meta_data = fopen(meta_file, "r");
    if (meta_data == NULL) {
        fprintf(stderr, "\"%s\" not available.\n", meta_file);
        exit(1);
    }
    fscanf(meta_data, " %d\n%d\n%d\n%d %d\n%d %d\n%d %d", &field_height, &field_width, &max_score, &bot1_x, &bot1_y, &bot_2x, &bot_2y, &bot_3x, &bot_3y);
    char *read = readingfile(filename);
    char **map = (char **) malloc(field_height * sizeof(char *));
    if (map == NULL) {
        printf("Memory error!\n");
        exit(1);
    }
    for (int i = 0; i < field_height; i++) {
        map[i] = (char *) malloc(field_width * sizeof(char));
        if (map[i] == NULL) {
            printf("Memory error!\n");
            exit(1);
        }
    }
    int k = 0;
    for (int i = 0; i < field_height; i++) {
        for (int j = 0; j < field_width; j++) {
            if (read[k] == '\n') {
                k++;
            }
            if (read[k] == '\0')
                break;
            map[i][j] = read[k];
            k++;
        }
    }
    return map;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Вывести символ с использованием заданной пары цветов в заданной позиции.///
static void con_charAt(int ch, int color, int x, int y) {
    con_gotoXY(x, y);
    con_setColor(color);
    con_outTxt("%c", ch);
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Инициализация цветов.///
static void init_colors() {
    con_initPair(COLOR_BORDER, CON_COLOR_BLACK, CON_COLOR_BLUE);
    con_initPair(COLOR_FIELD, CON_COLOR_GREEN, CON_COLOR_GREEN);
    con_initPair(COLOR_POINT, CON_COLOR_RED, CON_COLOR_GREEN);
    con_initPair(COLOR_BOT, CON_COLOR_RED, CON_COLOR_MAGENTA);
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Рисует героя по заданным символу, цвету и координатам.///
void draw_hero(hero *bot) {
    con_charAt(bot->symb, bot->colour, bot->pointx, bot->pointy);
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Рисование самого уровня.///
static void initial_draw(char **map, hero *main_hero, hero *bot1, hero *bot2, hero *bot3) {
    con_clearScr();
    con_gotoXY(TITLE_X, TITLE_Y);
    con_outTxt("Use arrows to move point, use Esc to exit.");
    {
        int i, j;
        for (j = 0; j < field_height; j++) {
            for (i = 0; i < field_width; i++) {
                int ch;
                int color;
                if (map[j][i] == '#') {
                    ch = CHAR_BORDER;
                    color = COLOR_BORDER;
                } else if (map[j][i] == '*') {
                    ch = CHAR_FOOD;
                    color = COLOR_FIELD;
                } else {
                    ch = CHAR_FIELD;
                    color = COLOR_FIELD;
                }
                con_charAt(ch, color, field_x + i, field_y + j);
            }
        }
    }
    draw_hero(main_hero);
    draw_hero(bot1);
    draw_hero(bot2);
    draw_hero(bot3);
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Выбор направления "пакмана" в зависимости от нажатой клавиши. Если нажат escape - выход из программы.///
int process_key(int key, hero *main_hero) {
    switch (key) {
        case CON_KEY_ESCAPE:
            return 1;

        case CON_KEY_UP:
            game_started = true;
            main_hero->dir = UP;
            break;

        case CON_KEY_DOWN:
            game_started = true;
            main_hero->dir = DOWN;
            break;

        case CON_KEY_LEFT:
            game_started = true;
            main_hero->dir = LEFT;
            break;

        case CON_KEY_RIGHT:
            game_started = true;
            main_hero->dir = RIGHT;
            break;
    }
    return 0;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Движения "пакмана" в зависимости от выделенного направления.///
/* Returns 1 if quit. */
void move(char **map, hero *main_hero) {
    int dx = 0;
    int dy = 0;
    switch (main_hero->dir) {
        case UP:
            if ((main_hero->pointy - 1 > field_y) &&
                (map[main_hero->pointy - 1 - field_y][main_hero->pointx - field_x] != '#')) {
                dy = -1;
            }
            break;

        case DOWN:
            if ((main_hero->pointy + 1 < field_y + field_height - 1) &&
                (map[main_hero->pointy + 1 - field_y][main_hero->pointx - field_x] != '#')) {
                dy = 1;
            }
            break;

        case LEFT:
            if ((main_hero->pointx - 1 > field_x) &&
                (map[main_hero->pointy - field_y][main_hero->pointx - 1 - field_x] != '#')) {
                dx = -1;
            }
            break;

        case RIGHT:
            if ((main_hero->pointx + 1 < field_x + field_width - 1) &&
                (map[main_hero->pointy - field_y][main_hero->pointx + 1 - field_x] != '#')) {
                dx = 1;
            }
            break;
    }
    if (dx != 0 || dy != 0) {
        if (map[main_hero->pointy - field_y + dy][main_hero->pointx - field_x + dx] == '*') {
            score++;
            map[main_hero->pointy - field_y + dy][main_hero->pointx - field_x + dx] = ' ';
        }
        con_charAt(CHAR_FIELD, COLOR_FIELD, main_hero->pointx, main_hero->pointy);
        main_hero->pointx += dx;
        main_hero->pointy += dy;
        draw_hero(main_hero);
        con_gotoXY(50, 1);
        printf("Score: %d", score);
    }
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Проверяет возможные передвижения бота в зависимости от расположения стенок.///
bool legal_move_bot(char **map, int new_x, int new_y) {
    if (map[new_y - TITLE_Y - FIELD_PADDING][new_x - TITLE_X - FIELD_PADDING] == CHAR_BORDER)
        return 0;
    return 1;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Движение бота.///
void move_bot(char **map, hero *bot, int new_x, int new_y) {
    if (legal_move_bot(map, new_x, new_y)) {
        if (map[new_y - TITLE_Y - FIELD_PADDING][new_x - TITLE_X - FIELD_PADDING] == '*') {
            con_charAt(CHAR_FOOD, COLOR_FIELD, bot->pointx, bot->pointy);
        }
        else con_charAt(CHAR_FIELD, COLOR_FIELD, bot->pointx, bot->pointy);
        bot->pointx = new_x;
        bot->pointy = new_y;
        draw_hero(bot);
    }
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Массив возможных направлений бота, где possible_noves[0] - вправо, [1] - влево, [2] - вверх, [3] - вниз.///
void Possible_moves(char **map, hero *bot) {
    int x = bot->pointx;
    int y = bot->pointy;
    con_gotoXY(x, y);
    if (legal_move_bot(map, x + 1, y) == 1) {
        bot->possible_moves[0] = 1;
    }
    if (legal_move_bot(map, x - 1, y) == 1) {
        bot->possible_moves[1] = 1;
    }
    if (legal_move_bot(map, x, y - 1) == 1) {
        bot->possible_moves[2] = 1;
    }
    if (legal_move_bot(map, x, y + 1) == 1) {
        bot->possible_moves[3] = 1;
    }
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Случайный выбор направления бота.///
void random_moving(hero *bot) {
    int *rand_possible_moves;
    int size = 0;
    bool flag = false;
    for (int i = 0; i < 4; ++i) {
        if (bot->possible_moves[i]) {
            size++;
            if (size == 1 && !flag){
                rand_possible_moves = (int*) malloc(sizeof(int));
                flag = true;
            } else {
                rand_possible_moves = (int*)realloc(rand_possible_moves, size * sizeof(int));
            }
            rand_possible_moves[size - 1] = i;
        }
    }
    srand(time(NULL));
    int idx = rand() % size;
    if (rand_possible_moves[idx] == 0)
        bot->dir = RIGHT;
    else if (rand_possible_moves[idx] == 1)
        bot->dir = LEFT;
    else if (rand_possible_moves[idx] == 2)
        bot->dir = UP;
    else
        bot->dir = DOWN;
    free(rand_possible_moves);
}
//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Столкновения бота и "пакмана".///
int check_collision(hero *player, hero *bot1) {
    if (player->pointy == bot1->pointy && player->pointx == bot1->pointx)
        return 1;
    return 0;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Удаление бота.///
void delete_bot(hero *bot) {
    free(bot->possible_moves);
    free(bot);
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Завершение игры: гибель или собрана вся еда.///
void finish(char **map, hero *player, hero *bot1, hero *bot2, hero *bot3) {
    if (score == max_score) {
        con_gotoXY(50, 1);
        printf("YOU WON!!!:)))))))))))))))");
        int last_move_time = clock();
        while (1) {
            int cur_time = clock();
            if (cur_time - last_move_time > MOVE_TIME_STEP * 5) {
                con_clearScr();
                con_deinit();
                delete_bot(bot1);
                delete_bot(bot2);
                delete_bot(bot3);
                free(map);
                exit(0);
            }
        }
    } else if (check_collision(player, bot1) || check_collision(player, bot2) || check_collision(player, bot3)) {
        con_gotoXY(50, 1);
        printf("YOU DIED!!!:((((((((((((((");
        int last_move_time = clock();
        while (1) {
            int cur_time = clock();
            if (cur_time - last_move_time > MOVE_TIME_STEP * 5) {
                con_clearScr();
                con_deinit();
                delete_bot(bot1);
                delete_bot(bot2);
                delete_bot(bot3);
                free(map);
                exit(0);
            }
        }
    }
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Очищает массив возможных направлений.///
void cleaning_possible_moves(hero* bot){
    for (int i = 0; i<4; i++)
        bot->possible_moves[i] = 0;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Вспомогательная функция для движения ботов в main.///
void moving_bots(hero* bot, char** map){
    Possible_moves(map, bot);
    random_moving(bot);
    if (bot->dir == UP)
        move_bot(map, bot, bot->pointx, bot->pointy - 1);
    if (bot->dir == DOWN)
        move_bot(map, bot, bot->pointx, bot->pointy + 1);
    if (bot->dir == LEFT)
        move_bot(map, bot, bot->pointx - 1, bot->pointy);
    if (bot->dir == RIGHT)
        move_bot(map, bot, bot->pointx + 1, bot->pointy);
    cleaning_possible_moves(bot);
}
//-------------------------------------------------------------//
//-------------------------------------------------------------//
///Игровой процесс.///
int main() {
//    srand(time(NULL));
    int quit = 0;
    char **map = read_map();

    con_init();
    con_hideCursor();
    init_colors();

    field_x = FIELD_PADDING + TITLE_X;
    field_y = FIELD_PADDING + TITLE_Y;
    game_started = false;

    int point_x = field_x + field_width / 2;
    int point_y = 23 + field_y;

    hero *player = create_hero(point_x, point_y, CHAR_POINT, COLOR_POINT);
    hero *bot1 = create_hero(bot1_x, bot1_y, CHAR_BOT1, COLOR_BOT);
    hero *bot2 = create_hero(bot_2x, bot_2y, CHAR_BOT2, COLOR_BOT);
    hero *bot3 = create_hero(bot_3x, bot_3y, CHAR_BOT3, COLOR_BOT);
    initial_draw((char **) map, player, bot1, bot2, bot3);
    con_gotoXY(bot1->pointx, bot1->pointy);
    Possible_moves(map, bot1);
    Possible_moves(map, bot2);
    Possible_moves(map, bot3);
    random_moving(bot1);
    random_moving(bot2);
    random_moving(bot3);

    int last_move_time = clock();

    while (!quit) {
        if (con_keyPressed()) {
            if (process_key(con_getKey(), player)) {
                quit = 1;
            }
        }

        if (game_started) {
            int cur_time = clock();
            if (cur_time - last_move_time > MOVE_TIME_STEP) {

                moving_bots(bot1, map);
                moving_bots(bot2, map);
                moving_bots(bot3, map);

                move((char **) map, player);
                last_move_time = cur_time;
            }
        }
        finish(map, player, bot1, bot2, bot3);
    }
}