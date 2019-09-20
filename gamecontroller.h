#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>

class QGraphicsScene;
class QKeyEvent;

class Snake;
class Food;
class Wall;

class GameController : public QObject
{
    Q_OBJECT
public:
    GameController(QGraphicsScene &scene, QObject *parent = nullptr);
    ~GameController();

    void snakeAteFood(Food *food);
//    void snakeHitWall(Snake *snake, Wall *wall);
    void snakeAteItself();

    bool isPause;

public slots:
    void pause();
    void resume();
    void gameOver();
//    void setMoveDirection(int dire);

signals:
    void paused();
    void resumed();
    void directionChanged(int dire);
    void gameOvered();

protected:
     bool eventFilter(QObject *object, QEvent *event);

private:
    void handleKeyPressed(QKeyEvent *event);
    void addNewFood();

    QTimer timer;
    QGraphicsScene &scene;

    int score;
    Snake *snake;
};

#endif // GAMECONTROLLER_H
