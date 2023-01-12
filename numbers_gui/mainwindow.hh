/* ###########################################################################
 # COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures          #
 # Project 4: 2048 Numbers game                                              #
 # File: mainwindow                                                          #
 # Description: Contains the MainWindow attributes and methods as well as    #
 #              the public and private slots the program and its widgets     #
 #              use.                                                         #
 # Notes: * All used widgets are listed as private attributes to ensure      #
 #        *   their deletion later                                           #
 #############################################################################
 *
 * Desc:
 *  This program generates a Qt Application game of 2048, a tile combining game
 * where the goal is to get from 2's to 2048 or another goal number.
 * The board is SIZE x SIZE, in this case 4x4, and every round the player choo-
 * ses a direction to which the tiles should fall. If they collide with a wall
 * or a different value, they will move as close as they can get. If they coll-
 * ide with a tile with same value, they will combine and double the value. The
 * tile will continue to move until a wall or other tile comes along.
 * (This means there's some recursion, see GameBoard::move().
 *  Game will end when the GOAL (orig 2048) is reached or new tile can't be
 * added to the board.
 *
 *  Writer:
 * Name: Vilhelmiina Rannikko
 * Student ID: H290652
 * TUNI user: vqvira
 * E-Mail: vilhelmiina.rannikko@tuni.fi
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "numbertile.hh"
#include "gameboard.hh"

#include <QMainWindow>

#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Constructor and destructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Initializer functions the QSpinBoxes, the gameboard, and buttons
    void initializeSpinBoxes();
    void initializeGrid();
    void initializeButtons();

    //keyPressEvent function to execute the GameBoard move-function if
    //WASD keys are pressed
    void keyPressEvent(QKeyEvent* event) override;


public slots:
    //Updates the numbers on the gameboard according to the inner GameBoard
    void updateBoard();

    //Sets the values from the QSpinBoxes as seed and goal values
    void setInputValues();

    //Resets the gameboard
    void resetGame();

private slots:
    //Updates the goalInfoLabel on what is the players current goal value
    void displayGoal();

private:
    Ui::MainWindow *ui;
    int seed;
    int goal;
    GameBoard board;
    std::vector<QLabel*> labels;
    QSpinBox* seedSpinBox;
    QSpinBox* goalSpinBox;
    QPushButton* startButton;
    QPushButton* resetButton;
    QLabel* statusLabel;
    QLabel* instructLabel;
    QLabel* seedLabel;
    QLabel* goalLabel;
    QLabel* goalInfoLabel;
    bool keysDisabled = false;
};
#endif // MAINWINDOW_HH
