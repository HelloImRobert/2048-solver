#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sstream>
#include <string>

#include "gamedata.h"
#include "aidata.h"
#include <QString>
#include <cmath>
#include <QtGui>
#include <windows.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void show_game();

    QString getstring(int);



private slots:
    void on_btn_newgame_clicked();

    void on_btn_up_clicked();

    void on_btn_left_clicked();

    void on_btn_down_clicked();

    void on_btn_right_clicked();

    void on_btn_start_clicked();

    void on_btn_stop_clicked();

private:
    Ui::MainWindow *ui;

    //testclass TestObj;

    AIdata AIgame;

    gamedata MainGame;

    void exec_move(int);

    void keyPressEvent(QKeyEvent *event);

    bool continue_thinking;

    void ai_loop();

};

#endif // MAINWINDOW_H
