#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

#include <constants.h>
#include "gamecontroller.h"
#include "food.h"
#include "snake.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    isPause(false),
    scene(scene),
    snake(new Snake(*this))
{
    timer.start( 1000/33 );

    addNewFood();

    scene.addItem(snake);
    scene.installEventFilter(this);

    QObject::connect(this, &GameController::directionChanged, this, &GameController::setMoveDirection);

    resume();
}

GameController::~GameController()
{
}

void GameController::snakeAteFood(Food *food)
{
    scene.removeItem(food);

    addNewFood();
}

//void GameController::snakeHitWall(Snake *snake, Wall *wall)
//{
//}

void GameController::snakeAteItself()
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::handleKeyPressed(QKeyEvent *event)
{
    if (!isPause)
        switch (event->key()) {
            case Qt::Key_Left:
//                snake->setMoveDirection(Snake::MoveLeft);
                emit directionChanged(1);
                break;
            case Qt::Key_Right:
//                snake->setMoveDirection(Snake::MoveRight);
                emit directionChanged(2);
                break;
            case Qt::Key_Up:
//                snake->setMoveDirection(Snake::MoveUp);
                emit directionChanged(3);
                break;
            case Qt::Key_Down:
//                snake->setMoveDirection(Snake::MoveDown);
                emit directionChanged(4);
                break;
            case Qt::Key_Space:
                pause();
                break;
        }
    else{
        resume();
    }
}

void GameController::setMoveDirection(int dire){

    switch (dire) {
    case 1:
        snake->setMoveDirection(Snake::MoveLeft);
        break;
    case 2:
        snake->setMoveDirection(Snake::MoveRight);
        break;
    case 3:
        snake->setMoveDirection(Snake::MoveUp);
        break;
    case 4:
        snake->setMoveDirection(Snake::MoveDown);
        break;
    default:
        return;
    }
}

void GameController::addNewFood()
{
    int x, y;

    do {
        x = (qrand() % 6)*(qrand()%2==0?-1:1);
        y = (qrand() % 6)*(qrand()%2==0?-1:1);
        x *= (TILE_SIZE*5);
        y *= (TILE_SIZE*5);
    } while (snake->shape().contains(snake->mapFromScene(QPointF(x + TILE_SIZE*2.5, y + TILE_SIZE*2.5))));
    qDebug()<<x<<y;
    Food *food = new Food(x, y);
    scene.addItem(food);
}

void GameController::gameOver()
{
    disconnect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    if (QMessageBox::Yes == QMessageBox::information(nullptr,
                            tr("Game Over"), tr("Again?"),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::Yes)) {
        connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
        scene.clear();

        snake = new Snake(*this);
        scene.addItem(snake);
        addNewFood();
    } else {
//        exit(0);
    }
}

void GameController::pause()
{
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
    isPause = true;
//    qDebug()<<"GameController::pause"<<isPause;
    emit paused();
}

void GameController::resume()
{
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
    connect(&timer, SIGNAL(timeout()),
            &scene, SLOT(advance()));
    isPause = false;
//    qDebug()<<"GameController::resume"<<isPause;
    emit resumed();

}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}
