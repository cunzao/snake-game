#include <QPainter>

#include "constants.h"
#include "gamecontroller.h"
#include "snake.h"
#include <QDebug>

static const qreal SNAKE_SIZE = TILE_SIZE*5;

Snake::Snake(GameController &acontroller) :
    head(0, 0),
    growing(7),
    speed(5),
    moveDirection(NoMove),
    controller(acontroller),
    directionChangedOneRound(false)
{

}

QRectF Snake::boundingRect() const
{
    qreal minX = head.x();
    qreal minY = head.y();
    qreal maxX = head.x();
    qreal maxY = head.y();

    foreach (QPointF p, tail) {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF tl = mapFromScene(QPointF(minX, minY));
    QPointF br = mapFromScene(QPointF(maxX, maxY));

    QRectF bound = QRectF(tl.x(),  // x
                          tl.y(),  // y
                          br.x() - tl.x() + SNAKE_SIZE,      // width
                          br.y() - tl.y() + SNAKE_SIZE       //height
                          );
    return bound;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));

    foreach (QPointF p, tail) {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }
//    qDebug()<<tail;
    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), Qt::darkGreen);
    painter->restore();
}

void Snake::setMoveDirection(Direction direction)
{
    if(directionChangedOneRound){
        qDebug()<<"按多了";
        return;
    }
    if (moveDirection == MoveLeft && direction == MoveRight)
        return;
    if (moveDirection == MoveRight && direction == MoveLeft)
        return;
    if (moveDirection == MoveUp && direction == MoveDown)
        return;
    if (moveDirection == MoveDown && direction == MoveUp)
        return;
    moveDirection = direction;
    directionChangedOneRound = true;
}

Snake::Direction Snake::currentDirection()
{
	return moveDirection;
}

void Snake::advance(int step)
{
    if (!step) {
        return;
    }
    if (tickCounter++ % speed != 0) {
        return;
    }
    if (moveDirection == NoMove) {
        return;
    }

    if (growing > 0) {
		QPointF tailPoint = head;
        tail << tailPoint;
        growing -= 1;
    } else {
        tail.removeFirst();
        tail << head;
    }

    switch (moveDirection) {
        case MoveLeft:
            moveLeft();
            break;
        case MoveRight:
            moveRight();
            break;
        case MoveUp:
            moveUp();
            break;
        case MoveDown:
            moveDown();
            break;
        default:
            return;
    }
//    qDebug()<<speed;
    setPos(head);
    handleCollisions();
    directionChangedOneRound = false;
}

void Snake::moveLeft()
{
    head.rx() -= SNAKE_SIZE;
    if (head.rx() < -TILE_SIZE*30) {
        head.rx() = TILE_SIZE*25;
    }
}

void Snake::moveRight()
{
    head.rx() += SNAKE_SIZE;
    if (head.rx() >= TILE_SIZE*30) {
        head.rx() = -TILE_SIZE*30;
    }
}

void Snake::moveUp()
{
    head.ry() -= SNAKE_SIZE;
    if (head.ry() < -TILE_SIZE*30) {
        head.ry() = TILE_SIZE*25;
    }
}

void Snake::moveDown()
{
    head.ry() += SNAKE_SIZE;
    if (head.ry() >= TILE_SIZE*30) {
        head.ry() = -TILE_SIZE*30;
    }
}

void Snake::handleCollisions()
{
    QList<QGraphicsItem *> collisions = collidingItems();

    // Check collisions with other objects on screen
    foreach (QGraphicsItem *collidingItem, collisions) {
        if (collidingItem->data(GD_Type) == GO_Food) {
            // Let GameController handle the event by putting another apple
            controller.snakeAteFood((Food *)collidingItem);
            growing += 1;
        }
    }

    // Check snake eating itself
    if (tail.contains(head)) {
        controller.snakeAteItself();
    }
}
