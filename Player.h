#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{
public:
    Player(const std::string& name, char sign);
    const std::string& getName() const;
    char getSign() const;
private:
    std::string name_;
    char sign_;
};

#endif // PLAYER_H
