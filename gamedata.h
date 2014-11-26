#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QMainWindow>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <vector>

class gamedata
{
public:
    gamedata();

    bool move(int);

    void reset_game();

    int get_score();

    int check_free();

    int fielddrop();

    int fielddrop(int);

    bool check_move(int);

    bool check_gameover();

    std::vector<std::vector<int> > game_field;

    int score;

private:

    bool move_left();

    bool move_right();

    bool move_up();

    bool move_down();

    int exp_to_num(int);

};

#endif // GAMEDATA_H
