#include "Player.h"

Player::Player(const std::string& name, char sign)
    :name_(name), sign_(sign){}

const std::string& Player::getName() const
{
    return name_;
}

char Player::getSign() const
{
    return sign_;
}

