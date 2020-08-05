#ifndef GameBoard_H
#define GameBoard_H

#include <QMainWindow>
#include <Player.h>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class GameBoard; }
QT_END_NAMESPACE

class GameBoard : public QMainWindow
{
    Q_OBJECT

public:
    GameBoard(QWidget *parent = nullptr);
    ~GameBoard();
    void swapPlayer();

    bool checkWinner();
    bool checkDraw();
    bool gameOver();

    void setLabel();
    void computerPlay();

private:
    Ui::GameBoard *ui;
    std::shared_ptr<Player> p1;
    std::shared_ptr<Player> p2;
    std::shared_ptr<Player> current;
    static int x;

private slots:
    void setValue();
    void reset();
    void multiPlayer();
    void singlePlayer();
    void mainMenu();
};
#endif // GameBoard_H
