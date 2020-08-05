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

    // Init players
    p1 = std::make_shared<Player>("Player1", 'X', true);
    p2 = std::make_shared<Player>("Player2", 'O', true);
    current = p1;

    // Connecting 9 buttons
    QPushButton* name[9];
    for(int i =0; i < 9; i++)
    {
        QString buttonName = "button_" + QString::number(i);
        name[i] = findChild<QPushButton*>(buttonName);
        connect(name[i], SIGNAL(released()), this, SLOT(setValue()));
    }

    setLabel();
    connect(ui->restart,  SIGNAL(released()), this, SLOT(reset()));
    connect(ui->mainMenu, SIGNAL(released()), this, SLOT(mainMenu()));




    connect(ui->real, SIGNAL(released()), this, SLOT(multiPlayer()));
    connect(ui->fake, SIGNAL(released()), this, SLOT(singlePlayer()));
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
}

void GameBoard::mainMenu()
{
    ui->stackedWidget->setCurrentIndex(0);
    reset();
}

void GameBoard::setValue()
{
    QPushButton* button = (QPushButton*)sender();
    if(button->text() != "X" && button->text() != "O")
    {
        bool is_reset = false;
        button->setText(QString(current->getSign()));

        if(gameOver())
        {
            reset();
            is_reset = true;
        } else
        {
            swapPlayer();
            setLabel();
        }

        //QCoreApplication::processEvents();

        if(!(current->getIsHuman()) && !is_reset)
        {
            qDebug() << "aaa";

            computerPlay();
        }
    }
}

void GameBoard::computerPlay()
{
    QPushButton* button;

    do{
        int num = current->genNumber();
        button = findChild<QPushButton*>("button_" + QString::number(num));
    }while(!(button->text() == ""));

    sleep(1);
    button->setText(current->getSign());

    if(gameOver())
    {
        reset();
    } else
    {
        swapPlayer();
        setLabel();
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

