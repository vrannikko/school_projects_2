/*
 #############################################################################
 # COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures          #
 # Project 4: 2048 Numbers game                                              #
 # File: mainwindow                                                          #
 # File: orienteeringmap                                                     #
 # Description: Contains the implementations of of all used methods.         #
 #############################################################################
*/

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>

const Coords DEFAULT_DIR = {0, 0};
const Coords LEFT = {0, -1};
const Coords UP = {-1, 0};
const Coords RIGHT = {0, 1};
const Coords DOWN = {1, 0};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeGrid();
    initializeButtons();
    initializeSpinBoxes();

    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetGame);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::setInputValues);

    connect(goalSpinBox,SIGNAL(valueChanged(int)),this,SLOT(displayGoal()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete seedSpinBox;
    delete goalSpinBox;
    delete resetButton;
    delete startButton;
    delete statusLabel;
    delete seedLabel;
    delete goalLabel;
    delete instructLabel;
    delete goalInfoLabel;

    for ( QLabel* x : labels )
    {
        delete x;
    }


}

void MainWindow::initializeSpinBoxes()
{

    seedSpinBox = new QSpinBox(this);
    goalSpinBox = new QSpinBox(this);

    //Setting dimensions on seedSpinBox and goalSpinBox, creating labels
    seedSpinBox->setGeometry(QRect(250, 40, 80, 30));
    seedSpinBox->setMaximum(300);
    goalSpinBox->setGeometry(QRect(250, 100, 80, 30));
    goalSpinBox->setMaximum(65536);

    seedLabel = new QLabel(this);
    goalLabel = new QLabel(this);
    seedLabel->setAlignment(Qt::AlignCenter);
    seedLabel->setGeometry(60, 10, 170, 100);
    seedLabel->setText("Insert wanted seed\nvalue for randomization:");

    goalLabel->setAlignment(Qt::AlignCenter);
    goalLabel->setGeometry(60, 70, 170, 100);
    goalLabel->setText("Insert a power of 2\nfor wanted goal value:");

    goalInfoLabel = new QLabel(this);
    goalInfoLabel->setAlignment(Qt::AlignCenter);
    goalInfoLabel->setGeometry(320, 80, 130, 70);
    goalInfoLabel->setText("");

}

void MainWindow::initializeGrid()
{

    //Assigning margin values as well as placing the grid on the interface
    //by looping through 4x4 QLabels and placing them next to eachother
    int x_margin = 270; // x coordinate
    int y_margin = 240; // y coordinate

    for ( int i = 0; i <= SIZE-1; i++ )
    {
        for ( int j = 0; j <= SIZE-1; j++)
        {
            QLabel* numLabel = new QLabel(this);
            numLabel->setText("");
            numLabel->setAlignment(Qt::AlignCenter);
            numLabel->setFrameStyle(QFrame::WinPanel | QFrame::Plain);
            numLabel->setGeometry(x_margin + (50*i), y_margin + (50*j), 50, 50);
            numLabel->setStyleSheet("background-color: mistyrose");

            //Saving labels to a vector for later use
            labels.push_back(numLabel);

        }
    }

    //Label for guiding player on how to play, positioning around halfway
    //the grid
    instructLabel = new QLabel(this);
    instructLabel->setText("Use WASD keys to play!");
    instructLabel->setAlignment(Qt::AlignCenter);
    instructLabel->setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    instructLabel->setGeometry(50, 290+(30*(SIZE/3)), 170, 40);
    instructLabel->setStyleSheet("background-color: peachpuff");

    //Label for displaying victory or loss
    statusLabel = new QLabel(this);
    statusLabel->setText("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setGeometry(570, 290+(30*(SIZE/3)), 150, 40);

}

void MainWindow::initializeButtons()
{

    //Creating and styling the buttons
    startButton = new QPushButton(this);
    resetButton = new QPushButton(this);

    startButton->setGeometry(QRect(520, 40, 80, 30));
    startButton->setStyleSheet("background-color: peachpuff");
    resetButton->setGeometry(QRect(520, 100, 80, 30));
    resetButton->setStyleSheet("background-color: peachpuff");

    startButton->setText("Start");
    resetButton->setText("Reset");
}

void MainWindow::setInputValues()
{
    keysDisabled = false;

    seed = seedSpinBox->value();

    //Initializing the gameboard with random values.
    board.init_empty();
    board.fill(seed);

    updateBoard();

    //Releasing resetButton if game was reseted, disabling
    //startButton to avoid player clicking in twice
    resetButton->setDisabled(false);
    startButton->setDisabled(true);


}

void MainWindow::keyPressEvent(QKeyEvent* event) {



    //If the key disabling flag is set false, keyPressEvent will trigger
    if( !keysDisabled )
    {
        Coords dir = DEFAULT_DIR;

        //Defining WASD keys to wanted directions
        if(event->key() == Qt::Key_S) { dir = RIGHT; }
        if(event->key() == Qt::Key_W) { dir = LEFT;  }
        if(event->key() == Qt::Key_D) { dir = DOWN;  }
        if(event->key() == Qt::Key_A) { dir = UP;    }

        //The board does a move and updates, with definitions for victory
        //and loss
        if( dir != DEFAULT_DIR )
        {
            if( board.move(dir, goal) )
            {
                keysDisabled = true;
                updateBoard();
                statusLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
                statusLabel->setStyleSheet("background-color: yellow");
                statusLabel->setText("You won! :-)");

            }
            else if( board.is_full() )
            {
                keysDisabled = true;
                statusLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
                statusLabel->setStyleSheet("background-color: orange");
                statusLabel->setText("You lost! :-(");
            }

            //In case the game is lost and the board is full, the board
            //will skip adding in a new value to avoid crashes
            if ( board.is_full() )
            {
                board.new_value(true);
            }
            else
            {
                board.new_value(false);
            }

            updateBoard();
        }
    }
    else
    {
        event->ignore();
    }




}

void MainWindow::resetGame()
{
    //Disabling WASD during resetting
    keysDisabled = true;

    //Setting everything to 0 to hide numbers from board
    for( auto y = 0; y < SIZE; ++y )
    {
        for( auto x = 0; x < SIZE; ++x )
        {
            board.board_.at(y).at(x)->value_ = 0;

        }
    }

    updateBoard();

    //Emptying memory of previous board.
    for( auto y = 0; y < SIZE; ++y )
    {
        for( auto x = 0; x < SIZE; ++x )
        {
            delete board.board_.at(y).at(x);
        }
    }

    //Creating a new GameBoard, hiding statusLabel
    board = GameBoard();
    statusLabel->setText("");
    statusLabel->setFrameStyle(QFrame::NoFrame);
    statusLabel->setStyleSheet("background-color: transparent");

    //Disabling resetButton to avoid player clicking it twice,
    //releasing startButton to begin new game
    resetButton->setDisabled(true);
    startButton->setDisabled(false);

}

void MainWindow::updateBoard()
{
    //Flag to manage labels array indexes
    int labelFlag = 0;

    //Looping through every NumberTile on the inner board, and setting
    //the NumberTile's value_ attribute visible on the numLabels.
    for( auto y = 0; y < SIZE; ++y )
    {
        for( auto x = 0; x < SIZE; ++x )
        {
            int value = board.board_.at(y).at(x)->value_;
            QString labelValue = QString::number(value);
            if ( value == 0 ) { labelValue = ""; }
            labels[labelFlag]->setText(labelValue);
            labelFlag++;
        }
    }
}

void MainWindow::displayGoal()
{
    int power = goalSpinBox->value();

    if ( power == 0 ) { goal = DEFAULT_GOAL; }
    else { goal = pow(2, power); }

    goalInfoLabel->setText("GOAL: " + QString::number(goal));
}
