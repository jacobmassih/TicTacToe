#include "Player.h"


Player::Player(const QString& name, const QChar& sign, bool isHuman)
    :name_(name), sign_(sign), isHuman_(isHuman){}

const QString& Player::getName() const
{
    return name_;
}

const QChar& Player::getSign() const
{
    return sign_;
}
void Player::setName(QString name)
{
    name_ = name;
}

void Player::setIsHuman(bool isHuman)
{
    isHuman_ = isHuman;
}

bool Player::getIsHuman() const
{
    return isHuman_;
}

