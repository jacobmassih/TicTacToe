#include "GameBoard.h"
#include "ui_gameBoard.h"
#include <QDebug>
#include <iostream>
#include <QMessageBox>

int GameBoard::x = 0;

GameBoard::GameBoard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameBoard), p1("Player1", 'X'), p2("Player2", 'O'), current(&p1)
{
    ui->setupUi(this);

    QPushButton* name[9];
    for(int i =0; i < 9; i++)
    {
        QString buttonName = "button_" + QString::number(i);
        name[i] = findChild<QPushButton*>(buttonName);
        connect(name[i], SIGNAL(released()), this, SLOT(setValue()));
    }

    connect(ui->restart, SIGNAL(released()), this, SLOT(reset()));

    setLabel();
    GameBoard::setWindowTitle("Tic Tac Toe v1");
}

GameBoard::~GameBoard()
{
    delete ui;
}

Player* GameBoard::swap()
{
    x++;
    if(x % 2 == 0)
        return &p1;
    else
        return &p2;
}

void GameBoard::setValue()
{
    QPushButton* button = (QPushButton*)sender();
    if(button->text() != "X" && button->text() != "O")
    {
        button->setText(QString(current->getSign()));
        checkOverAll();
        current = swap();
        setLabel();
    }
}

void GameBoard::checkOverAll()
{
    if(CheckWinner())
    {
        QMessageBox* winner = new QMessageBox();
        QString player = QString(current->getSign()) + " - " + QString::fromStdString(current->getName());
        winner->setText(player + " is the winner.\nCongratulations!");
        winner->exec();
        reset();
    }
    else if(checkDraw())
    {
        QMessageBox* draw = new QMessageBox();
        draw->setText("The GameBoard ended with a draw!\nThere is no winner.");
        draw->exec();
        reset();
    }
}
bool GameBoard::CheckWinner()
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
    current = &p1;
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
    QString currentPlayer = QString(current->getSign()) + " - " + QString::fromStdString(current->getName()) + "'s turn";
    ui->player_disp->setText(currentPlayer);
}
