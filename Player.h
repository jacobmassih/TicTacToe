#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player
{
public:
    Player(const QString& name, const QChar& sign, bool isHuman);

    // Getters
    const QString& getName() const;
    const QChar& getSign() const;
    bool getIsHuman() const;

    // Setters
    void setName(QString name);
    void setIsHuman(bool isHuman);

    int genNumber();
private:
    QString name_;
    QChar sign_;
    bool isHuman_;
};

#endif // PLAYER_H
