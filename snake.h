#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>

class GameController;

class Snake : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    enum Direction {
        NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

//    Snake(GameController &acontroller);
    Snake(GameController &acontroller, QObject *parent = nullptr);
    ~Snake();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setMoveDirection(Direction direction);
    Direction currentDirection();

protected:
    void advance(int step);

public slots:
    void setMoveDire(int dire);
    void snakeDie();

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
};

#endif // SNAKE_H
