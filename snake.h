#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>

class GameController;

class Snake : public QGraphicsItem
{
public:
    enum Direction {
        NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

    Snake(GameController &acontroller);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setMoveDirection(Direction direction);
//    void setMoveDirection(int);
	Direction currentDirection();

protected:
    void advance(int step);

//public slots:
//    void setMoveDirection(int dire);

private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void handleCollisions();

    QPointF        head;
    int            growing;
    int            speed;
    QList<QPointF> tail;
    int            tickCounter;
    Direction      moveDirection;
    GameController &controller;
    bool directionChangedOneRound;
};

#endif // SNAKE_H
