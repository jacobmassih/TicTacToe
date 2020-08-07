#include "GameBoard.h"
#include "ui_gameBoard.h"
#include <QMessageBox>
#include <unistd.h>
#include <QDebug>

int GameBoard::x = 0;

GameBoard::GameBoard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameBoard)
{
    ui->setupUi(this);
    GameBoard::setWindowTitle("Tic Tac Toe v1");

    // Init player private attributes
    p1 = std::make_shared<Player>("Player1", 'X', true);
    p2 = std::make_shared<Player>("Player2", 'O', true);
    current = p1;

    // Connecting 9 buttons
    QPushButton* buttons[9];
    for(int i =0; i < 9; i++)
    {
        QString buttonName = "button_" + QString::number(i);
        buttons[i] = findChild<QPushButton*>(buttonName);
        connect(buttons[i], SIGNAL(released()), this, SLOT(setValue()));
    }

    // Page 0 buttons.
    connect(ui->multi, SIGNAL(released()), this, SLOT(multiPlayer()));
    connect(ui->single, SIGNAL(released()), this, SLOT(singlePlayer()));
    setLabel();

    // Page 1 buttons
    connect(ui->restart,  SIGNAL(released()), this, SLOT(reset()));
    connect(ui->mainMenu, SIGNAL(released()), this, SLOT(mainMenu()));
}


GameBoard::~GameBoard()
{
    delete ui;
}


void GameBoard::multiPlayer()
{
    ui->stackedWidget->setCurrentIndex(1);
    p2->setName("Player2");
    p2->setIsHuman(true);
}


void GameBoard::swapPlayer()
{
    x++;

    if(x % 2 == 0)
        current = p1;
    else
        current = p2;

    setLabel();
}

void GameBoard::mainMenu()
{
    ui->stackedWidget->setCurrentIndex(0);
    reset();
}

void GameBoard::setValue()
{
    QPushButton* clicked = (QPushButton*)sender();
    if(clicked->text() != "X" && clicked->text() != "O")
    {
        bool isReset = false;
        clicked->setText(QString(current->getSign()));

        if(gameOver()){
            reset();
            isReset = true;
        } else {
            swapPlayer();
        }

        QCoreApplication::processEvents();

        if(!(current->getIsHuman()) && !isReset)
        {
            aiMove();
        }
    }
}

void GameBoard::aiMove()
{
    int bestScore = -1000;
    QPushButton* bestMove;
    QPushButton* buttons[9];
    for(int i = 0; i < 9; i++)
    {
        buttons[i] = GameBoard::findChild<QPushButton*>("button_" + QString::number(i));

        if(buttons[i]->text() == "")
        {
            buttons[i]->setText(current->getSign());
            int score = minimax(0, false);
            buttons[i]->setText("");
            if(score > bestScore)
            {
                bestScore = score;
                bestMove = buttons[i];
            }
        }
    }

    bestMove->setText(current->getSign());
    sleep(1);
    if(gameOver())
        reset();
    else
        swapPlayer();
}


int GameBoard::minimax(int depth, bool isMaximizing)
{
    if(checkWinner())
        if(isMaximizing)
            return -10;
        else
            return 10;
    else if(checkDraw())
        return 0;

    if(gameOver())
        reset();


    if(isMaximizing)
    {
        int bestScore = -1000;
        QPushButton* buttons[9];

        for(int i = 0; i < 9; i++)
        {
            buttons[i] = GameBoard::findChild<QPushButton*>("button_" + QString::number(i));
            if(buttons[i]->text() == "")
            {
                buttons[i]->setText(p2->getSign());
                int score = minimax(depth + 1, false) - depth;
                buttons[i]->setText("");
                bestScore = std::max(bestScore, score);
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = 1000;
        QPushButton* buttons[9];

        for(int i = 0; i < 9; i++)
        {
            buttons[i] = GameBoard::findChild<QPushButton*>("button_" + QString::number(i));
            if(buttons[i]->text() == "")
            {
                buttons[i]->setText(p1->getSign());
                int score = minimax(depth + 1, true) + depth;
                buttons[i]->setText("");
                bestScore = std::min(bestScore, score);
            }
        }
    return bestScore;
    }
}


void GameBoard::singlePlayer()
{
    ui->stackedWidget->setCurrentIndex(1);
    p2->setName("Computer");
    p2->setIsHuman(false);
}

bool GameBoard::gameOver()
{
    if(checkWinner())
    {
        QMessageBox* winner = new QMessageBox();
        QString player = QString(current->getSign()) + " - " + current->getName();
        winner->setGeometry(875, 275, 250, 200);
        winner->setText(player + " is the winner.\nCongratulations!");
        winner->exec();
        delete winner;
        return true;
    }
    else if(checkDraw())
    {
        QMessageBox* draw = new QMessageBox();
        draw->setGeometry(850, 275, 250, 200);
        draw->setText("The GameBoard ended with a draw!\nThere is no winner.");
        draw->exec();
        delete draw;
        return true;
    }
    return false;
}

bool GameBoard::checkWinner()
{
    //Horizontal
    if(ui->button_0->text() == ui->button_1->text() &&
       ui->button_1->text() == ui->button_2->text() &&
       ui->button_0->text() != "")
            return true;

    if(ui->button_3->text() == ui->button_4->text() &&
       ui->button_4->text() == ui->button_5->text() &&
       ui->button_3->text() != "")
            return true;

    if(ui->button_6->text() == ui->button_7->text() &&
       ui->button_7->text() == ui->button_8->text() &&
       ui->button_6->text() != "")
            return true;

    //Vertical
    if(ui->button_0->text() == ui->button_3->text() &&
       ui->button_3->text() == ui->button_6->text() &&
       ui->button_0->text() != "")
            return true;
    if(ui->button_1->text() == ui->button_4->text() &&
       ui->button_4->text() == ui->button_7->text() &&
       ui->button_1->text() != "")
            return true;
    if(ui->button_2->text() == ui->button_5->text() &&
       ui->button_5->text() == ui->button_8->text() &&
       ui->button_2->text() != "")
            return true;

    //Diagonals
    if(ui->button_0->text() == ui->button_4->text() &&
       ui->button_4->text() == ui->button_8->text() &&
       ui->button_0->text() != "")
            return true;
    if(ui->button_2->text() == ui->button_4->text() &&
       ui->button_4->text() == ui->button_6->text() &&
       ui->button_2->text() != "")
            return true;

    return false;
}

bool GameBoard::checkDraw()
{
    QPushButton* button[9];
    for(int i =0; i < 9; i++)
    {
        QString x = "button_" + QString::number(i);
        button[i] = findChild<QPushButton*>(x);

        if(button[i]->text() == "")
        {
            return false;
        }
    }
    return true;
}

void GameBoard::reset()
{
    current = p1;
    x = 0;
    QPushButton* button[9];
    for(int i =0; i < 9; i++)
    {
        QString x = "button_" + QString::number(i);
        button[i] = findChild<QPushButton*>(x);

        button[i]->setText("");
    }
    setLabel();
}

void GameBoard::setLabel()
{
    QString currentPlayer = QString(current->getSign()) + " - " + current->getName() + "'s turn";
    ui->player_disp->setText(currentPlayer);
}

