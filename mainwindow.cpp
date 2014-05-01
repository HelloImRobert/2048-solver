#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->dspl_status->setText("have fun!");
    MainWindow::show_game();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_game() //refresh game board with current values
{

    ui->Scoreboard->display(MainWindow::MainGame.score); //set scoreboard

    ui->fld_00->setText(MainWindow::getstring(MainGame.game_field[0][0])); //convert values to strings and write onto board
    ui->fld_01->setText(MainWindow::getstring(MainGame.game_field[0][1]));
    ui->fld_02->setText(MainWindow::getstring(MainGame.game_field[0][2]));
    ui->fld_03->setText(MainWindow::getstring(MainGame.game_field[0][3]));
    ui->fld_10->setText(MainWindow::getstring(MainGame.game_field[1][0]));
    ui->fld_11->setText(MainWindow::getstring(MainGame.game_field[1][1]));
    ui->fld_12->setText(MainWindow::getstring(MainGame.game_field[1][2]));
    ui->fld_13->setText(MainWindow::getstring(MainGame.game_field[1][3]));
    ui->fld_20->setText(MainWindow::getstring(MainGame.game_field[2][0]));
    ui->fld_21->setText(MainWindow::getstring(MainGame.game_field[2][1]));
    ui->fld_22->setText(MainWindow::getstring(MainGame.game_field[2][2]));
    ui->fld_23->setText(MainWindow::getstring(MainGame.game_field[2][3]));
    ui->fld_30->setText(MainWindow::getstring(MainGame.game_field[3][0]));
    ui->fld_31->setText(MainWindow::getstring(MainGame.game_field[3][1]));
    ui->fld_32->setText(MainWindow::getstring(MainGame.game_field[3][2]));
    ui->fld_33->setText(MainWindow::getstring(MainGame.game_field[3][3]));
}

QString MainWindow::getstring(int intvalue) //converts exponents of board values into number strings
{
    QString stringvalue;

    if (intvalue == 0)
        stringvalue = "";
    else
        stringvalue = QString::number(std::pow(2, intvalue)); //only the exponent is saved in array -> expands here

    return stringvalue;
}

void MainWindow::on_btn_newgame_clicked() //start a new game
{
    MainWindow::MainGame.reset_game();
    ui->dspl_status->setText("have fun!");
    MainWindow::show_game();
}

void MainWindow::on_btn_up_clicked()
{
    MainWindow::exec_move(0);
}

void MainWindow::on_btn_left_clicked()
{
    MainWindow::exec_move(3);
}

void MainWindow::on_btn_down_clicked()
{
    MainWindow::exec_move(2);
}

void MainWindow::on_btn_right_clicked()
{
    MainWindow::exec_move(1);
}

void MainWindow::exec_move(int direction)
{
    if (MainWindow::MainGame.check_gameover() == true) {
        ui->dspl_status->setText("!!!Game Over!!!");
        continue_thinking = false;
    }
    else if(MainWindow::MainGame.check_move(direction) == false){
        ui->dspl_status->setText("illegal move");
        continue_thinking = false;
    }
    else
    {
        MainWindow::MainGame.move(direction);
        MainWindow::show_game();
        ui->dspl_status->setText("good move!");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Up)
        MainWindow::exec_move(0);
    else if (ev->key() == Qt::Key_Down)
        MainWindow::exec_move(2);
    else if (ev->key() == Qt::Key_Left)
        MainWindow::exec_move(3);
    else if (ev->key() == Qt::Key_Right)
        MainWindow::exec_move(1);
}


void MainWindow::on_btn_start_clicked()
{
    continue_thinking = true;

    MainWindow::ai_loop();

}

void MainWindow::on_btn_stop_clicked()
{
    continue_thinking = false;
}

void MainWindow::ai_loop()
{
    for (int i = 0; i<2000 ; i++)
    {
        int depth = 0;

        depth = ui->spinBox->value();

        if (depth < 0)
            depth = 0;

        if (depth > 3)
            depth = 3;

        MainWindow::exec_move(MainWindow::AIgame.think(MainGame.game_field, depth));

        qApp->processEvents();
        Sleep(1000);
        if (continue_thinking == false)
            break;
    }
}
