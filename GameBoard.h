#ifndef GameBoard_H
#define GameBoard_H

#include <QMainWindow>
#include <Player.h>
QT_BEGIN_NAMESPACE
namespace Ui { class GameBoard; }
QT_END_NAMESPACE

class GameBoard : public QMainWindow
{
    Q_OBJECT

public:
    GameBoard(QWidget *parent = nullptr);
    ~GameBoard();
    Player* swap();
    bool CheckWinner();
    bool checkDraw();
    void checkOverAll();
    void setLabel();

private:
    Ui::GameBoard *ui;
    Player p1;
    Player p2;
    Player* current;
    static int x;

private slots:
    void setValue();
    void reset();

};
#endif // GameBoard_H
