#include "aidata.h"


Scorefield::Scorefield(Node &inputnode)
{
    this->fieldarr.resize(4);
    this->fieldarr[0].resize(4);
    this->fieldarr[1].resize(4);
    this->fieldarr[2].resize(4);
    this->fieldarr[3].resize(4);

    this->free_slots = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->fieldarr[i][j].value = inputnode.fieldarr[i][j];

            if (inputnode.fieldarr[i][j] == 0) //count empty slots
                this->free_slots++;

            this->fieldarr[i][j].up_two.score_1 = 0;
            this->fieldarr[i][j].up_two.score_2 = 0;
            this->fieldarr[i][j].up_two.deathchance = 0.0;
            this->fieldarr[i][j].up_four.score_1 = 0;
            this->fieldarr[i][j].up_four.score_2 = 0;
            this->fieldarr[i][j].up_four.deathchance = 0.0;

            this->fieldarr[i][j].right_two.score_1 = 0;
            this->fieldarr[i][j].right_two.score_2 = 0;
            this->fieldarr[i][j].right_two.deathchance = 0.0;
            this->fieldarr[i][j].right_four.score_1 = 0;
            this->fieldarr[i][j].right_four.score_2 = 0;
            this->fieldarr[i][j].right_four.deathchance = 0.0;

            this->fieldarr[i][j].down_two.score_1 = 0;
            this->fieldarr[i][j].down_two.score_2 = 0;
            this->fieldarr[i][j].down_two.deathchance = 0.0;
            this->fieldarr[i][j].down_four.score_1 = 0;
            this->fieldarr[i][j].down_four.score_2 = 0;
            this->fieldarr[i][j].down_four.deathchance = 0.0;

            this->fieldarr[i][j].left_two.score_1 = 0;
            this->fieldarr[i][j].left_two.score_2 = 0;
            this->fieldarr[i][j].left_two.deathchance = 0.0;
            this->fieldarr[i][j].left_four.score_1 = 0;
            this->fieldarr[i][j].left_four.score_2 = 0;
            this->fieldarr[i][j].left_four.deathchance = 0.0;
        }
    }
}

AIdata::AIdata()
{
    //Node initNode;
    //Solverstack.resize(5000, Node());

    /*
    for (unsigned int i = 0; i < Solverstack.size(); i++ ) // init AI data stack
    {
        Solverstack[i].lvl = 0;
        Solverstack[i].score_1 = 0;
        Solverstack[i].score_2 = 0;
        Solverstack[i].deathchance = 0.0;
        Solverstack[i].resolve_direction = 0;
        Solverstack[i].parent_id = 0;
        Solverstack[i].child_id = 0;

        Solverstack[i].fieldarr.resize(4);

        for (int j = 0; j < 4; j++ )
        {
            Solverstack[i].fieldarr[j].resize(4);
            for (int k = 0; k < 4; k++ )
            {
                Solverstack[i].fieldarr[j][k] = 0;
            }
        }
    }
    */
}


bool AIdata::move_up(Node &inputnode)
{

    int floor;
    int ceiling;
    bool returnval = false;

    if (this->check_move(inputnode,0) == true) {
        returnval = true;
        for(int j = 0; j < 4 ; j++) {//for each colum

            floor = 0;
            ceiling = 1;

            while((ceiling <= 3)&&(floor < 3)) {

                if(inputnode.fieldarr[ceiling][j] == 0){ //is ceiling zero?
                    ceiling++; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[ceiling][j] == inputnode.fieldarr[floor][j]){ //merger possible?
                    inputnode.fieldarr[ceiling][j] = 0;
                    inputnode.fieldarr[floor][j] += 1; // floor value *2
                    floor++;
                    ceiling++;
                    continue;
                }

                if(inputnode.fieldarr[floor][j] == 0){
                    inputnode.fieldarr[floor][j] = inputnode.fieldarr[ceiling][j];
                    inputnode.fieldarr[ceiling][j] = 0;
                    ceiling++;
                }
                else {
                    floor++;
                    if(floor == ceiling)
                        ceiling++;
                }
            }
        }
    }
    return returnval;
}

bool AIdata::move_down(Node &inputnode)
{

    int floor;
    int ceiling;
    bool returnval = false;


    if (this->check_move(inputnode, 2) == true)
    {
        returnval = true;
        for(int j = 0; j < 4 ; j++)
        {//for each colum

            floor = 3;
            ceiling = 2;

            while((ceiling >= 0)&&(floor > 0))
            {

                if(inputnode.fieldarr[ceiling][j] == 0)
                { //is ceiling zero?
                    ceiling--; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[ceiling][j] == inputnode.fieldarr[floor][j])
                { //merger possible?
                    inputnode.fieldarr[ceiling][j] = 0;
                    inputnode.fieldarr[floor][j] += 1; // floor value *2
                    floor--;
                    ceiling--;
                    continue;
                }

                if(inputnode.fieldarr[floor][j] == 0)
                {
                    inputnode.fieldarr[floor][j] = inputnode.fieldarr[ceiling][j];
                    inputnode.fieldarr[ceiling][j] = 0;
                    ceiling--;
                }
                else
                {
                    floor--;
                    if(floor == ceiling)
                        ceiling--;
                }
            }
        }
    }
    return returnval;
}

bool AIdata::move_left(Node &inputnode)
{
    int floor;
    int ceiling;
    bool returnval = false;

    if (this->check_move(inputnode,3) == true) {
        returnval = true;
        for(int i = 0; i < 4 ; i++) {//for each colum

            floor = 0;
            ceiling = 1;

            while((ceiling <= 3)&&(floor < 3)) {

                if(inputnode.fieldarr [i][ceiling] == 0){ //is ceiling zero?
                    ceiling++; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[i][ceiling] == inputnode.fieldarr[i][floor]){ //merger possible?
                    inputnode.fieldarr[i][ceiling] = 0;
                    inputnode.fieldarr[i][floor] += 1; // floor value *2
                    floor++;
                    ceiling++;
                    continue;
                }

                if(inputnode.fieldarr[i][floor] == 0){
                    inputnode.fieldarr[i][floor] = inputnode.fieldarr[i][ceiling];
                    inputnode.fieldarr[i][ceiling] = 0;
                    ceiling++;
                }
                else {
                    floor++;
                    if(floor == ceiling)
                        ceiling++;
                }
            }
        }

    }
    return returnval;
}

bool AIdata::move_right(Node &inputnode)
{
    int floor;
    int ceiling;
    bool returnval = false;

    if (this->check_move(inputnode,1) == true) {
        returnval = true;
        for(int i = 0; i < 4 ; i++) {//for each colum

            floor = 3;
            ceiling = 2;

            while((ceiling >= 0)&&(floor > 0)) {

                if(inputnode.fieldarr [i][ceiling] == 0){ //is ceiling zero?
                    ceiling--; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[i][ceiling] == inputnode.fieldarr[i][floor]){ //merger possible?
                    inputnode.fieldarr[i][ceiling] = 0;
                    inputnode.fieldarr[i][floor] += 1; // floor value *2
                    floor--;
                    ceiling--;
                    continue;
                }

                if(inputnode.fieldarr[i][floor] == 0){
                    inputnode.fieldarr[i][floor] = inputnode.fieldarr[i][ceiling];
                    inputnode.fieldarr[i][ceiling] = 0;
                    ceiling--;
                }
                else {
                    floor--;
                    if(floor == ceiling)
                        ceiling--;
                }
            }
        }
    }
    return returnval;
}

bool AIdata::check_move(Node &inputnode, int direction)
{   // check if legal move in desired direction possible
    // directions 0 = up, 1 = right, 2 = down, 3 = left

    if (direction == 0) {//up
        for(int j = 0; j < 4 ; j++) //colum
        {
            for(int i = 0; i < 3; i++)//row
            {
                if ((inputnode.fieldarr[i][j] == 0)&&(inputnode.fieldarr[i+1][j] != 0)) //slide possible?
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i+1][j]))) // merger possible?
                {
                    return true;
                }
            }
        }
    }
    else if (direction == 1){//right
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 3; j > 0; j--)// colum
            {
                if ((inputnode.fieldarr[i][j] == 0)&&(inputnode.fieldarr[i][j-1] != 0))
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i][j-1])))
                {
                    return true;
                }
            }
        }
    }
    else if (direction == 2){//down
        for(int j = 0; j < 4 ; j++) //colum
        {
            for(int i = 3; i > 0; i--)//row
            {
                if ((inputnode.fieldarr[i][j] == 0)&&(inputnode.fieldarr[i-1][j] != 0)) //slide possible?
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i-1][j]))) // merger possible?
                {
                    return true;
                }
            }
        }
    }
    else if (direction == 3){ //left
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 0; j < 3; j++)// colum
            {
                if ((inputnode.fieldarr[i][j] == 0) && (inputnode.fieldarr[i][j+1] != 0))
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i][j+1])))
                {
                    return true;
                }
            }
        }
    }
    return false;
}


Scores_struct AIdata::check_score (Node inputnode, int depth) //recursive score checker
{
    Scores_struct scores;
    scores.score_1 = 0.0;
    scores.score_2 = 0;
    scores.deathchance = 0.0;

    Node passing_node(inputnode);

    // TODO:: how to choose on endgame?
    // TODO:: branching optimization

    if(depth > 0)// further recursion needed?
    {
        int empty_slots;
        Scores_struct scores_temp, worst_scores_up, worst_scores_down, worst_scores_left, worst_scores_right;
        bool move_done = false;

        empty_slots = AIdata::check_empty(inputnode);

        worst_scores_down.score_1 = 16; //init with the best possible values
        worst_scores_down.score_2 = 1.0;
        worst_scores_down.deathchance = 0.0;

        worst_scores_up.score_1 = 16;
        worst_scores_up.score_2 = 1.0;
        worst_scores_up.deathchance = 0.0;

        worst_scores_left.score_1 = 16;
        worst_scores_left.score_2 = 1.0;
        worst_scores_left.deathchance = 0.0;

        worst_scores_right.score_1 = 16;
        worst_scores_right.score_2 = 1.0;
        worst_scores_right.deathchance = 0.0;

        for(int i = 0 ; i < 4 ; i++) // iterate scorefield
        {
            for (int j = 0; j < 4 ; j++)
            {
                if (inputnode.fieldarr[i][j] == 0) // empty? fielddrop possible?
                {
                    move_done = false;

                    // do for up
                    // drop 2
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if(move_up(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_up.score_1) //now check if this has the worst score of all
                            worst_scores_up.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_up.score_2)
                            worst_scores_up.score_2 = scores_temp.score_2;
                    }
                    worst_scores_up.deathchance = worst_scores_up.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance


                    //down
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if(move_down(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_down.score_1)
                            worst_scores_down.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_down.score_2)
                            worst_scores_down.score_2 = scores_temp.score_2;
                    }
                    worst_scores_down.deathchance = worst_scores_down.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance


                    //left
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if(move_left(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_left.score_1)
                            worst_scores_left.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_left.score_2)
                            worst_scores_left.score_2 = scores_temp.score_2;
                    }
                    worst_scores_left.deathchance = worst_scores_left.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance


                    //right
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if(move_right(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_right.score_1)
                            worst_scores_right.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_right.score_2)
                            worst_scores_right.score_2 = scores_temp.score_2;
                    }
                    worst_scores_right.deathchance = worst_scores_right.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance

                    //check for gameover (no move was possible => full deathchance for this possible drop)
                    if (move_done == false)
                    {
                        worst_scores_up.deathchance = worst_scores_up.deathchance + (0.666666 / empty_slots);
                        worst_scores_down.deathchance = worst_scores_down.deathchance + (0.666666 / empty_slots);
                        worst_scores_left.deathchance = worst_scores_left.deathchance + (0.666666  / empty_slots);
                        worst_scores_right.deathchance = worst_scores_right.deathchance + (0.666666 / empty_slots);
                    }


                    move_done = false;

                    //drop 4
                    //up
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_up(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_up.score_1)
                            worst_scores_up.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_up.score_2)
                            worst_scores_up.score_2 = scores_temp.score_2;
                    }
                    worst_scores_up.deathchance = worst_scores_up.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);


                    // down
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_down(passing_node))//move possible?
                    {
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_down.score_1)
                            worst_scores_down.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_down.score_2)
                            worst_scores_down.score_2 = scores_temp.score_2;
                    }
                    worst_scores_down.deathchance = worst_scores_down.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);


                    //  left
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_left(passing_node))//move possible?
                    {
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_left.score_1)
                            worst_scores_left.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_left.score_2)
                            worst_scores_left.score_2 = scores_temp.score_2;
                    }
                    worst_scores_left.deathchance = worst_scores_left.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);


                    //right
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_right(passing_node))//move possible?
                    {
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_right.score_1)
                            worst_scores_right.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_right.score_2)
                            worst_scores_right.score_2 = scores_temp.score_2;
                    }
                    worst_scores_right.deathchance = worst_scores_right.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);

                    //check for gameover
                    if (move_done == false)
                    {
                        worst_scores_up.deathchance = worst_scores_up.deathchance + (0.333334  / empty_slots);
                        worst_scores_down.deathchance = worst_scores_down.deathchance + (0.333334  / empty_slots);
                        worst_scores_left.deathchance = worst_scores_left.deathchance + (0.333334  / empty_slots);
                        worst_scores_right.deathchance = worst_scores_right.deathchance + (0.333334 / empty_slots);
                    }
                }
            }
        }

        // ----- choose best direction by score -----
        scores.score_1 = worst_scores_up.score_1;
        scores.score_2 = worst_scores_up.score_2;
        scores.deathchance = worst_scores_up.deathchance;

        //check if down better
        if(scores.score_1 < worst_scores_down.score_1) //looking for best score...
            scores.score_1 = worst_scores_down.score_1;

        if(scores.score_2 < worst_scores_down.score_2)
            scores.score_2 = worst_scores_down.score_2;

        if(scores.deathchance > worst_scores_down.deathchance)//... looking for lowest deathchance
            scores.deathchance = worst_scores_down.deathchance;

        //check left
        if(scores.score_1 < worst_scores_left.score_1)
            scores.score_1 = worst_scores_left.score_1;

        if(scores.score_2 < worst_scores_left.score_2)
            scores.score_2 = worst_scores_left.score_2;

        if(scores.deathchance > worst_scores_left.deathchance)
            scores.deathchance = worst_scores_left.deathchance;

        //check right
        if(scores.score_1 < worst_scores_right.score_1)
            scores.score_1 = worst_scores_right.score_1;

        if(scores.score_2 < worst_scores_right.score_2)
            scores.score_2 = worst_scores_right.score_2;

        if(scores.deathchance > worst_scores_right.deathchance)
            scores.deathchance = worst_scores_right.deathchance;

    }
    else //if no recursion needed calculate and return scores
    {
        AIdata::ScoreCalc.get_scores(inputnode);
        scores.score_1 = inputnode.score_1;
        scores.score_2 = inputnode.score_2;
    }

    return scores;
}

Scores_struct AIdata::check_score_deep (Node inputnode, int depth) //recursive score checker
{

}




void AIdata::copy_to_stack(const Node& input_node, int stack_position)
{
    //    //this->Solverstack[stack_position].score = input_node.score;
}

bool AIdata::check_gameover(const Node &inputnode)
{   //false -> move possible ; true -> gameover

    bool returnval = true;

    if (returnval == true) {//up
        for(int j = 0; j < 4 ; j++) //colum
        {
            if (inputnode.fieldarr[3][j] == 0) //any free slot? - last row not covered by the for loop
            {
                returnval = false;
                break;
            }

            for(int i = 0; i < 3; i++)//row
            {
                if (inputnode.fieldarr[i][j] == 0) //any free slot?
                {
                    returnval = false;
                    break;
                }
                else if (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i+1][j])) // merger possible?
                {
                    returnval = false;
                    break;
                }
            }
            if (returnval == false)
                break;
        }
    }

    if (returnval == true){ //left - check for mergers in other direction
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 0; j < 3; j++)// colum
            {
                if (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i][j+1]))
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

int AIdata::check_empty (const Node &inputnode)
{
    int returnval = 0;
    for(int i = 0; i < 4 ; i++)
    {
        for (int j = 0; j < 4 ; j++)
        {
            if(inputnode.fieldarr[i][j] == 0)
                returnval++;
        }
    }

    return returnval;
}

int AIdata::think(std::vector< std::vector<int> > &gamefield, int depth)
{
    // TODO:: BUG: AI makes illegal moven when every move ultimatively ends in certain (calculated) death

    Node startvalues; //create Nodes to pass along and init with starting values
    Node whenup;
    Node whenright;
    Node whendown;
    Node whenleft;

    int bestmovedirection = 0;
    double bestscore_1 = 0.0;
    double bestscore_2 = 0.0;
    double lowestdeathchance = 1.0;

    Scores_struct temp_scores;
    temp_scores.deathchance = 0.0;
    temp_scores.score_1 = 0;
    temp_scores.score_2 = 0;

    for(int i = 0; i < 4 ; i++)
    {
        for (int j = 0; j < 4 ; j++)
        {
            startvalues.fieldarr[i][j] = gamefield[i][j];
        }
    }

    AIdata::copy_node(startvalues, whenup);
    AIdata::copy_node(startvalues, whenright);
    AIdata::copy_node(startvalues, whendown);
    AIdata::copy_node(startvalues, whenleft);



    // now execute moves and evaluate the results with the score checker

    if(move_up(whenup))//up
    {
        temp_scores = AIdata::check_score(whenup, depth);

        bestmovedirection = 0;
        bestscore_1 = temp_scores.score_1;
        bestscore_2 = temp_scores.score_2;
        lowestdeathchance = temp_scores.deathchance;
    }



    if(move_right(whenright))//right
    {
        temp_scores = AIdata::check_score(whenright, depth);

        //take this direction if deathchances are equal and score_2 is better or if deathchance is lower
        if (((temp_scores.score_2 > bestscore_2) && (temp_scores.deathchance <= lowestdeathchance))||(temp_scores.deathchance < lowestdeathchance))
        {
            bestmovedirection = 1;
            bestscore_2 = temp_scores.score_2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
        else if ((temp_scores.score_2 >= bestscore_2) && (temp_scores.deathchance <= lowestdeathchance) && (temp_scores.score_1 > bestscore_1)) //if all things are equal differenciate through score_1
        {
            bestmovedirection = 1;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
    }



    if(move_down(whendown))//down
    {
        temp_scores = AIdata::check_score(whendown, depth);

        if (((temp_scores.score_2 > bestscore_2) && (temp_scores.deathchance <= lowestdeathchance))||(temp_scores.deathchance < lowestdeathchance))
        {
            bestmovedirection = 2;
            bestscore_2 = temp_scores.score_2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
        else if ((temp_scores.score_2 >= bestscore_2) && (temp_scores.deathchance <= lowestdeathchance) && (temp_scores.score_1 > bestscore_1))
        {
            bestmovedirection = 2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
    }



    if(move_left(whenleft))//left
    {
        temp_scores = AIdata::check_score(whenleft, depth);

        if (((temp_scores.score_2 > bestscore_2) && (temp_scores.deathchance <= lowestdeathchance))||(temp_scores.deathchance < lowestdeathchance))
        {
            bestmovedirection = 3;
            bestscore_2 = temp_scores.score_2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
        else if ((temp_scores.score_2 >= bestscore_2) && (temp_scores.deathchance <= lowestdeathchance) && (temp_scores.score_1 > bestscore_1))
        {
            bestmovedirection = 3;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
    }

    return bestmovedirection;
}


void AIdata::copy_node(Node &inputnode, Node &outputnode)
{


    outputnode.score_1 = inputnode.score_1;
    outputnode.score_2 = inputnode.score_2;


    for(int i = 0; i < 4 ; i++)
    {
        for (int j = 0; j < 4 ; j++)
        {
            outputnode.fieldarr[i][j] = inputnode.fieldarr[i][j];
        }
    }
}
