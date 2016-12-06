#ifndef TRAIN_H
#define TRAIN_H

#include <QObject>
#include <thread>
#include <chrono>
#include <vector>
#include "semaphore.h"

struct Position {
    int x, y;
    Position() : x(0), y(0) {}
    Position(int _x, int _y) : x(_x), y(_y) {}
};

struct Shape {
    Position coord;
    int width, height;
    Shape() : width(140), height(100) {}
    Shape(Position _coord, int _width = 140, int _height = 100)
        : coord(_coord), width(_width), height(_height) {}
    int left() {
        return coord.x;
    }
    int right() {
        return coord.x + width;
    }
    int top() {
        return coord.y;
    }
    int bottom() {
        return coord.y + height;
    }
};

enum Direction {CLOCKWISE, ANTICLOCKWISE};

class Train : public QObject {
    Q_OBJECT
 public:
    Train(int, Shape, Position, std::vector<Semaphore *> *, Direction = Direction::CLOCKWISE);
    ~Train();
    void start();
    void move();
    void run();
    void verifySection();
    void setSpeed(int);
    void setEnable(bool);

 signals:
    void updateGUI(int, int, int);

 private:
   int id;
   Shape shape;
   Position currPos;
   std::thread threadTrem;
   int speed;
   bool enable;
   Direction move_direction;
   std::vector<Semaphore *> *sems;
};

#endif // TRAIN_H
