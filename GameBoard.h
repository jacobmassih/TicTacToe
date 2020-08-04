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
    void swap();
    bool CheckWinner();
    bool checkDraw();
    bool checkOverAll();
    void setLabel();
    bool checkReset();

private:
    Ui::GameBoard *ui;
    std::shared_ptr<Player> p1;
    std::shared_ptr<Player> p2;
    std::shared_ptr<Player> current;
    static int x;

private slots:
    void setValue();
    void reset();
    void setPage();
    void initMainPage();
    void setComputer();
    void computerPlay();
};
#endif // GameBoard_H
