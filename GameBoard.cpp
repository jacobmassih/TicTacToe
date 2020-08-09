#include "GameBoard.h"
#include "ui_gameBoard.h"
#include <QMessageBox>
#include <unistd.h>

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
    for(int i =0; i < 9; i++)
    {
        buttons[i] = findChild<QPushButton*>("button_" + QString::number(i));
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

void GameBoard::setLabel()
{
    QString currentPlayer = QString(current->getSign()) + " - " + current->getName() + "'s turn";
    ui->player_disp->setText(currentPlayer);
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

bool GameBoard::checkWinner() const
{
    // Horizontal
    for(int i = 0; i <=6; i+=3)
        if(buttons[i]->text() != "" &&
           buttons[i]->text() == buttons[i+1]->text() &&
           buttons[i+1]->text() == buttons[i+2]->text())
            return true;

    //Vertical
    for(int  i = 0; i <= 2; i++)
        if(buttons[i]->text() != "" &&
           buttons[i]->text() == buttons[i+3]->text() &&
           buttons[i+3]->text() == buttons[i+6]->text())
            return true;

    //Diagonals
    if(buttons[0]->text() != "" &&
       buttons[0]->text() == buttons[4]->text() &&
       buttons[4]->text() == buttons[8]->text())
        return true;

    if(buttons[2]->text() != "" &&
       buttons[2]->text() == buttons[4]->text() &&
       buttons[4]->text() == buttons[6]->text())
        return true;

    return false;
}

bool GameBoard::checkDraw() const
{
    for(int i =0; i < 9; i++)
        if(buttons[i]->text() == "")
            return false;

    return true;
}

bool GameBoard::gameOver() const
{
    if(checkWinner())
    {
        std::unique_ptr<QMessageBox> winner = std::make_unique<QMessageBox>();
        QString player = QString(current->getSign()) + " - " + current->getName();
        winner->setGeometry(875, 275, 250, 200);
        winner->setText(player + " is the winner.\nCongratulations!");
        winner->exec();
        return true;
    }
    else if(checkDraw())
    {
        std::unique_ptr<QMessageBox> draw = std::make_unique<QMessageBox>();
        draw->setGeometry(850, 275, 250, 200);
        draw->setText("The GameBoard ended with a draw!\nThere is no winner.");
        draw->exec();
        return true;
    }

    return false;
}

void GameBoard::multiPlayer()
{
    ui->stackedWidget->setCurrentIndex(1);
    p2->setName("Player2");
    p2->setIsHuman(true);
}

void GameBoard::aiMove()
{
    int bestScore = -1000;
    QPushButton* bestMove;
    for(int i = 0; i < 9; i++)
    {
        if(buttons[i]->text() == "")
        {
            buttons[i]->setText(current->getSign());
            int score = minimax(0, -1000, 1000, false);
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

int GameBoard::minimax(int depth, int alpha, int beta, bool isMaximizing)
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

        for(int i = 0; i < 9; i++)
        {
            if(buttons[i]->text() == "")
            {
                buttons[i]->setText(p2->getSign());
                int score = minimax(depth + 1, alpha, beta, false) - depth;
                buttons[i]->setText("");
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, score);
                if(beta <= alpha)
                    break;
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = 1000;

        for(int i = 0; i < 9; i++)
        {
            if(buttons[i]->text() == "")
            {
                buttons[i]->setText(p1->getSign());
                int score = minimax(depth + 1, alpha, beta, true) + depth;
                buttons[i]->setText("");
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, score);
                if(beta <= alpha)
                    break;
            }
        }
    return bestScore;
    }
}


// private slots

void GameBoard::reset()
{
    current = p1;
    x = 0;
    for(int i =0; i < 9; i++)
        buttons[i]->setText("");

    setLabel();
}

void GameBoard::mainMenu()
{
    ui->stackedWidget->setCurrentIndex(0);
    reset();
}

void GameBoard::singlePlayer()
{
    ui->stackedWidget->setCurrentIndex(1);
    p2->setName("Computer");
    p2->setIsHuman(false);
}

void GameBoard::setValue()
{
    QPushButton* clickedButton = (QPushButton*)sender();
    if(clickedButton->text() == "")
    {
        bool isReset = false;
        clickedButton->setText(QString(current->getSign()));

        if(gameOver())
        {
            reset();
            isReset = true;
        }
        else
        {
            swapPlayer();
        }

        QCoreApplication::processEvents();

        if(!(current->getIsHuman()) && !isReset)
        {
            aiMove();
        }
    }
}

