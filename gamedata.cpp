#include "gamedata.h"

gamedata::gamedata()
{
    this->game_field.resize(4);
    this->game_field[0].resize(4);
    this->game_field[1].resize(4);
    this->game_field[2].resize(4);
    this->game_field[3].resize(4);

    gamedata::reset_game();
    //std::srand(time(NULL));
    std::srand(0); //debug --- make sure to always get the same results    
}


void gamedata::reset_game() //set game field values and score to 0, make two field drops
{
    score = 0;

    for(int i=0 ; i<=3; i++) // reset field
    {
        for(int j=0; j<=3; j++)
        {
            this->game_field[i][j]=0;
        }
    }

    this->fielddrop(1);//make sure max one 4 is being dropped
    this->fielddrop();

//    // DEBUG
//    this->fielddrop(3);
//    this->fielddrop(1);
//    this->fielddrop(4);
//    this->fielddrop(4);
//    this->fielddrop(3);
//    this->fielddrop();
//    this->fielddrop();
//    this->fielddrop();
//    this->fielddrop();
}

int gamedata::get_score() // returns current score
{
    return score;
}

int gamedata::check_free() //check how many free slots are available on the game field
{
    int returnval = 0;

    for(int i = 0; i < 4 ; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if (this->game_field[i][j] == 0)
                returnval++;
        }
    }

    return returnval;
}

int gamedata::fielddrop()//write RANDOM value into random empty slot
{
    int returnval = 0;

    //check if there's an empty field
    if (this->check_free() > 0)
    {
        while (returnval == 0) // find empty slot
        {
            int i = 0;
            int x = 0;
            int y = 0;

            x = std::rand()%4;
            y = std::rand()%4;

            if (this->game_field[x][y] == 0)// is slot empty?
            {
                if((std::rand()%3) == 2){//decide if dropping a 2( = 2^1) or a 4( = 2^2) with a chance of 2:1
                    this->game_field[x][y] = 2;
                    returnval = 2;
                }
                else{
                    this->game_field[x][y] = 1;
                    returnval = 1;
                }
                break;
            }

            i++;

            if (i>1000000)//sanity check
                break;
        }
    }

    return returnval; //returns dropped value
}

int gamedata::fielddrop(int request)//write REQUESTED (exponent!) value into random empty slot
{
    int returnval = 0;

    //check if there's an empty field
    if (this->check_free() > 0)
    {
        while (true)
        {
            int i = 0;
            int x = 0;
            int y = 0;

            x = std::rand()%4;
            y = std::rand()%4;

            if (this->game_field[x][y] == 0)//is slot empty?
            {
                this->game_field[x][y] = request;
                returnval = request;
                break;
            }

            i++;

            if (i>1000000)//sanity check
                break;
        }
    }

    return returnval; //returns dropped value
}

int gamedata::exp_to_num(int invalue) //converts exponents of board values into number
{
    int outvalue = 0;

    if (invalue == 0)
        outvalue = 0;
    else
        outvalue = std::pow(2, invalue); //only the exponent is saved in array -> expands here

    return outvalue;
}

bool gamedata::check_move(int direction)
{   // check if legal move in desired direction possible
    // directions 0 = up, 1 = right, 2 = down, 3 = left

    bool returnval = false;


    if (direction == 0) {//up
        for(int j = 0; j < 4 ; j++) //colum
        {
            for(int i = 0; i < 3; i++)//row
            {
                if ((this->game_field[i][j] == 0)&&(this->game_field[i+1][j] != 0)) //slide possible?
                {
                    returnval = true;
                    break;
                }
                if ((this->game_field[i][j] != 0) && (this->game_field[i][j] == (this->game_field[i+1][j]))) // merger possible?
                {
                    returnval = true;
                    break;
                }
            }
            if (returnval == true)
                break;
        }
    }
    else if (direction == 1){//right
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 3; j > 0; j--)// colum
            {
                if ((this->game_field[i][j] == 0)&&(this->game_field[i][j-1] != 0))
                {
                    returnval = true;
                    break;
                }
                if ((this->game_field[i][j] != 0) && (this->game_field[i][j] == (this->game_field[i][j-1])))
                {
                    returnval = true;
                    break;
                }
            }
            if (returnval == true)
                break;
        }
    }
    else if (direction == 2){//down
        for(int j = 0; j < 4 ; j++) //colum
        {
            for(int i = 3; i > 0; i--)//row
            {
                if ((this->game_field[i][j] == 0)&&(this->game_field[i-1][j] != 0)) //slide possible?
                {
                    returnval = true;
                    break;
                }
                if ((this->game_field[i][j] != 0) && (this->game_field[i][j] == (this->game_field[i-1][j]))) // merger possible?
                {
                    returnval = true;
                    break;
                }
            }
            if (returnval == true)
                break;
        }
    }
    else if (direction == 3){ //left
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 0; j < 3; j++)// colum
            {
                if ((this->game_field[i][j] == 0) && (this->game_field[i][j+1] != 0))
                {
                    returnval = true;
                    break;
                }
                if ((this->game_field[i][j] != 0) && (this->game_field[i][j] == (this->game_field[i][j+1])))
                {
                    returnval = true;
                    break;
                }
            }
            if (returnval == true)
                break;
        }
    }


    return returnval;
}



bool gamedata::check_gameover()
{   //false -> move possible ; true -> gameover

    bool returnval = true;

    if (returnval == true) {//up
        for(int j = 0; j < 4 ; j++) //colum
        {
            if (this->game_field[3][j] == 0) //any free slot? - last row not covered by the for loop
            {
                returnval = false;
                break;
            }

            for(int i = 0; i < 3; i++)//row
            {
                if (this->game_field[i][j] == 0) //any free slot?
                {
                    returnval = false;
                    break;
                }
                if (this->game_field[i][j] == (this->game_field[i+1][j])) // merger possible?
                {
                    returnval = false;
                    break;
                }
            }
            if (returnval == false)
                break;
        }
    }

    if (returnval == true){ //left
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 0; j < 3; j++)// colum
            {
                if (this->game_field[i][j] == (this->game_field[i][j+1]))
                {
                    returnval = false;
                    break;
                }
            }
            if (returnval == false)
                break;
        }
    }

    return returnval;
}


bool gamedata::move(int direction){//make move

    bool returnval = false;

    if (direction == 0) {
        returnval = this->move_up();
    }
    else if (direction == 1){
        returnval = this->move_right();
    }
    else if (direction == 2){
        returnval = this->move_down();
    }
    else if (direction == 3){
        returnval = this->move_left();
    }

    return returnval;
}


bool gamedata::move_down() //do downwards move, calc score, fielddrop
{
    bool returnval = false;
    int floor;
    int ceiling;

    if (this->check_move(2) == true) {
        for(int j = 0; j < 4 ; j++) {//for each colum

            floor = 3;
            ceiling = 2;

            while((ceiling >= 0)&&(floor > 0)) {

                if(this->game_field[ceiling][j] == 0){ //is ceiling zero?
                    ceiling--; //then goto next slot
                    continue;
                }

                if(this->game_field[ceiling][j] == this->game_field[floor][j]){ //merger possible?
                    this->game_field[ceiling][j] = 0;
                    this->game_field[floor][j] += 1; // floor value *2
                    this->score += std::pow(2,this->game_field[floor][j]);
                    floor--;
                    ceiling--;
                    continue;
                }

                if(game_field[floor][j] == 0){
                    this->game_field[floor][j] = this->game_field[ceiling][j];
                    this->game_field[ceiling][j] = 0;
                    ceiling--;
                }
                else {
                    floor--;
                    if(floor == ceiling)
                        ceiling--;
                }
            }
        }

        this->fielddrop();
    }


    return returnval;
}

bool gamedata::move_up(){

    bool returnval = false;
    int floor;
    int ceiling;

    if (this->check_move(0) == true) {
        for(int j = 0; j < 4 ; j++) {//for each colum

            floor = 0;
            ceiling = 1;

            while((ceiling <= 3)&&(floor < 3)) {

                if(this->game_field[ceiling][j] == 0){ //is ceiling zero?
                    ceiling++; //then goto next slot
                    continue;
                }

                if(this->game_field[ceiling][j] == this->game_field[floor][j]){ //merger possible?
                    this->game_field[ceiling][j] = 0;
                    this->game_field[floor][j] += 1; // floor value *2
                    this->score += std::pow(2,this->game_field[floor][j]); //calculate score
                    floor++;
                    ceiling++;
                    continue;
                }

                if(game_field[floor][j] == 0){
                    this->game_field[floor][j] = this->game_field[ceiling][j];
                    this->game_field[ceiling][j] = 0;
                    ceiling++;
                }
                else {
                    floor++;
                    if(floor == ceiling)
                        ceiling++;
                }
            }
        }

        this->fielddrop();
    }


    return returnval;
}

bool gamedata::move_left(){

    bool returnval = false;
    int floor;
    int ceiling;

    if (this->check_move(3) == true) {
        for(int i = 0; i < 4 ; i++) {//for each colum

            floor = 0;
            ceiling = 1;

            while((ceiling <= 3)&&(floor < 3)) {

                if(this->game_field [i][ceiling] == 0){ //is ceiling zero?
                    ceiling++; //then goto next slot
                    continue;
                }

                if(this->game_field[i][ceiling] == this->game_field[i][floor]){ //merger possible?
                    this->game_field[i][ceiling] = 0;
                    this->game_field[i][floor] += 1; // floor value *2
                    this->score += std::pow(2,this->game_field[i][floor]); //calculate score
                    floor++;
                    ceiling++;
                    continue;
                }

                if(game_field[i][floor] == 0){
                    this->game_field[i][floor] = this->game_field[i][ceiling];
                    this->game_field[i][ceiling] = 0;
                    ceiling++;
                }
                else {
                    floor++;
                    if(floor == ceiling)
                        ceiling++;
                }
            }
        }

        this->fielddrop();
    }


    return returnval;
}


bool gamedata::move_right(){
    bool returnval = false;
    int floor;
    int ceiling;

    if (this->check_move(1) == true) {
        for(int i = 0; i < 4 ; i++) {//for each colum

            floor = 3;
            ceiling = 2;

            while((ceiling >= 0)&&(floor > 0)) {

                if(this->game_field [i][ceiling] == 0){ //is ceiling zero?
                    ceiling--; //then goto next slot
                    continue;
                }

                if(this->game_field[i][ceiling] == this->game_field[i][floor]){ //merger possible?
                    this->game_field[i][ceiling] = 0;
                    this->game_field[i][floor] += 1; // floor value *2
                    this->score += std::pow(2,this->game_field[i][floor]); //calculate score
                    floor--;
                    ceiling--;
                    continue;
                }

                if(game_field[i][floor] == 0){
                    this->game_field[i][floor] = this->game_field[i][ceiling];
                    this->game_field[i][ceiling] = 0;
                    ceiling--;
                }
                else {
                    floor--;
                    if(floor == ceiling)
                        ceiling--;
                }
            }
        }

        this->fielddrop();
    }


    return returnval;
}
