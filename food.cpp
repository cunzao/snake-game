#include <QPainter>

#include "constants.h"
#include "food.h"

static const qreal FOOD_RADIUS = 3.0*TILE_SIZE/2;

Food::Food(qreal x, qreal y)
{
    setPos(x, y);
    setData(GD_Type, GO_Food);
}

QRectF Food::boundingRect() const
{
    return QRectF(-TILE_SIZE*5,    -TILE_SIZE*5,
                   TILE_SIZE*10, TILE_SIZE*10);
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::red);

    painter->restore();
}

QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(TILE_SIZE*2.5, TILE_SIZE*2.5), FOOD_RADIUS, FOOD_RADIUS);
    return p;
}
